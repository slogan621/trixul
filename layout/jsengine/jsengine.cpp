/*

Trixul Cross-Platform GUI Toolkit

Copyright (c) 2006, Syd Logan
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* Neither the name trixul nor the names of its contributors may be used to
endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "jsengine.h"

#include <cstring>

#if defined(XP_WIN)
#include <windows.h>   // for OutputDebugString
#else
#include <stdio.h>
#endif

#define LAZY_STANDARD_CLASSES

static JSEngine *js = NULL;

JSEngine *
JSEngine::GetJSEngine()
{
    if (!js)
        js = new JSEngine(); 
    return js;
}

static JSBool
global_enumerate(JSContext *cx, JSObject *obj)
{
#ifdef LAZY_STANDARD_CLASSES
    return JS_EnumerateStandardClasses(cx, obj);
#else
    return JS_TRUE;
#endif
}

static JSBool
global_resolve(JSContext *cx, JSObject *obj, jsval id, uintN flags, 
    JSObject **objp)
{
#ifdef LAZY_STANDARD_CLASSES
    if ((flags & JSRESOLVE_ASSIGNING) == 0) {
        JSBool resolved;

        if (!JS_ResolveStandardClass(cx, obj, id, &resolved))
            return JS_FALSE;
        if (resolved) {
            *objp = obj;
            return JS_TRUE;
        }
    }
#endif

#if defined(SHELL_HACK) && defined(DEBUG) && defined(XP_UNIX)
    if ((flags & (JSRESOLVE_QUALIFIED | JSRESOLVE_ASSIGNING)) == 0) {
        /*
         * Do this expensive hack only for unoptimized Unix builds, which are
         * not used for benchmarking.
         */
        char *path, *comp, *full;
        const char *name;
        JSBool ok, found;
        JSFunction *fun;

        if (!JSVAL_IS_STRING(id))
            return JS_TRUE;
        path = getenv("PATH");
        if (!path)
            return JS_TRUE;
        path = JS_strdup(cx, path);
        if (!path)
            return JS_FALSE;
        name = JS_GetStringBytes(JSVAL_TO_STRING(id));
        ok = JS_TRUE;
        for (comp = strtok(path, ":"); comp; comp = strtok(NULL, ":")) {
            if (*comp != '\0') {
                full = JS_smprintf("%s/%s", comp, name);
                if (!full) {
                    JS_ReportOutOfMemory(cx);
                    ok = JS_FALSE;
                    break;
                }
            } else {
                full = (char *)name;
            }
            found = (access(full, X_OK) == 0);
            if (*comp != '\0')
                free(full);
            if (found) {
                fun = JS_DefineFunction(cx, obj, name, Exec, 0,
                                        JSPROP_ENUMERATE);
                ok = (fun != NULL);
                if (ok)
                    *objp = obj;
                break;
            }
        }
        JS_free(cx, path);
        return ok;
    }
#else
    return JS_TRUE;
#endif
}

// Look at Load() in js/src/xpconnect/shell/xpcshell.cpp 

JSClass global_class = {
    "global", JSCLASS_NEW_RESOLVE,
    JS_PropertyStub,  JS_PropertyStub,
    JS_PropertyStub,  JS_PropertyStub,
    global_enumerate, (JSResolveOp) global_resolve,
    JS_ConvertStub,   JS_FinalizeStub
};

JS_STATIC_DLL_CALLBACK(void)
ErrorReporter(JSContext *cx, const char *message, JSErrorReport *report)
{
    if (message)
        printf("%s\n", message);
}

JS_STATIC_DLL_CALLBACK(JSBool)
QuitApp(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    AppListener *app;
    
    app = js->GetApp();
    if (app) {
        app->HandleQuit();
        return JS_TRUE;
    }
    return JS_FALSE;
}

JS_STATIC_DLL_CALLBACK(JSBool)
Dump(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSString *str;
    if (!argc)
        return JS_TRUE;

    str = JS_ValueToString(cx, argv[0]);
    if (!str)
        return JS_FALSE;

    char *bytes = JS_GetStringBytes(str);
    bytes = strdup(bytes);

#if defined(XP_WIN)
    OutputDebugString(bytes);
#else
    fputs(bytes, stderr);
#endif
    free(bytes);
    return JS_TRUE;
}

static JSFunctionSpec glob_functions[] = {
    {"dump", Dump, 1},
    {"quit", QuitApp, 1},
    {0}
};

// inverse of InitEngine()

PRStatus
JSEngine::ReleaseEngine()
{
    FreeContextPool();
    JS_BeginRequest(m_cx);
    JS_RemoveRoot(m_cx, &m_glob);
    JS_EndRequest(m_cx);
    JS_DestroyContextMaybeGC(m_cx);
    if (m_rt) {
        JS_DestroyRuntime(m_rt);
        m_rt = NULL;
        JS_ShutDown();
    }
    return PR_SUCCESS;
}

PRStatus
JSEngine::InitEngine()
{
    /* set up global JS variables, including global and custom objects */

    //JSVersion version;
    JSContext *cx;
    JSObject *glob;
    JSBool builtins;

    /* initialize the JS run time, and return result in rt */
    m_rt = JS_NewRuntime(8L * 1024L * 1024L);

    /* if rt does not have a value, end the program here */
    if (!m_rt)
        return PR_FAILURE;

    /* create a context and associate it with the JS run time */
    m_cx = cx = JS_NewContext(m_rt, 8192);

    JS_SetOptions(cx,
        JS_GetOptions(cx)|JSOPTION_STRICT|JSOPTION_WERROR); 

    /* if cx does not have a value, end the program here */
    if (cx == NULL)
        return PR_FAILURE;

    JS_SetErrorReporter(cx, ErrorReporter);

    JS_BeginRequest(cx);
    /* create the super global object here */
    m_glob = glob = JS_NewObject(cx, &global_class, NULL, NULL);
    JS_EndRequest(cx);

    JS_BeginRequest(m_cx);
    JS_AddNamedRoot(m_cx, &m_glob, "superglobal");
    JS_EndRequest(m_cx);

    JS_BeginRequest(cx);
    /* initialize the built-in JS objects and the global object */
    builtins = JS_InitStandardClasses(cx, glob);

    JS_DefineFunctions(cx, m_glob, glob_functions);
    JS_EndRequest(cx);

    CreateContextPool();
    return PR_SUCCESS;
}

JSObject *
JSEngine::GetGlobalObject()
{
    return m_glob;
}

JSObject *
JSEngine::NewSuperGlobal(JSContext *cx)
{
    JSObject *glob;

    /* create the super global object here */
    JS_BeginRequest(cx);
    glob = JS_NewObject(cx, &global_class, m_glob, m_glob);
    JS_EndRequest(cx);

#if defined(NOT_YET) 
    // XXX let's see if things survive well enough without rooting the
    // object (rooting keeps it from the GC, but leads to warnings
    // about lingering roots when we release the engine). The alternative
    // would be to keep a list of rooted objects and then call JS_RemoveRoot
    // when no longer needed. The upside for doing so would be to avoid
    // "mysterious" crashes that the Mozilla docs say may occur if objects
    // are not rooted. The downside is we aren't explicitly removing these
    // objects, they should not be going away until we release the context
    // they are bound to (e.g., JS_DestroyContext(m_cx) is called).
    JS_BeginRequest(m_cx);
    JS_AddRoot(m_cx, &glob);
    JS_EndRequest(m_cx);
#endif

    return glob;
}

JSContext *
JSEngine::GetGlobalContext()
{
    return(m_cx);
}

PRStatus
JSEngine::CreateContextPool()
{
    JSContext *cx;

    for (int i = 0; i < m_numContexts; i++) {
        // create a pool of contexts
        cx = JS_NewContext(m_rt, 8192);
        JS_SetOptions(cx,
            JS_GetOptions(cx)|JSOPTION_STRICT|JSOPTION_WERROR); 
        m_pool.push_back(cx);
    }
    return PR_SUCCESS;
}

PRStatus
JSEngine::FreeContextPool()
{
    for (list<JSContext *>::iterator iter = m_pool.begin(); iter != m_pool.end(); ++iter) 
        JS_DestroyContextMaybeGC(*iter);
    m_pool.clear();

    for (list<JSContext *>::iterator iter = m_recycle.begin(); iter != m_recycle.end(); ++iter) 
        JS_DestroyContextMaybeGC(*iter);
    m_recycle.clear();
    return PR_SUCCESS;
}

JSContext *
JSEngine::GetNewContext()
{
    ReclaimContexts();
    JSContext *cx;

    if (m_pool.size() == 0) 
        return NULL;

    // remove a context from the pool

    cx = m_pool.front();
    m_pool.pop_front();
    return cx;
}

void
JSEngine::FreeContext(JSContext *cx)
{
    m_recycle.push_back(cx);
}

void
JSEngine::ReclaimContexts()
{
    std::list<JSContext *>::iterator iter;

    for (iter = m_recycle.begin(); iter != m_recycle.end(); ++iter) {
        // XXX danger here is context has objects that we don't want
        // to reuse. Need a way to either destroy the context, and add
        // a brand new one into the pool, or return the context to the
        // pool such that its contained objects are released
        m_pool.push_back(*iter);
    }
    m_recycle.clear();
}

