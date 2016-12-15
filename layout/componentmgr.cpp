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

#include "componentmgr.h"
#include "component.h"

#include <plstr.h>
#include "prio.h"
#include "prlink.h"

#include "plgetcwd.h"

static void StartElement(void *userData, const char *name, const char **atts);
static void EndElement(void *userData, const char *name);

static ComponentManager *instance = NULL;

Component *
ComponentManager::GetComponentByJSObject(JSObject *obj)
{
    map<string, Component *>::iterator iter;
    Component *ret = NULL;

    for (iter = m_components.begin(); iter != m_components.end(); ++iter) {
        Component *comp = (*iter).second;

        if (comp) {
            JSObject *jsobj = comp->GetJSObject();
            if (jsobj == obj) {
                ret = comp;
                break;
            }
        }
    }
    return ret;
}

ComponentManager* 
ComponentManager::GetInstance()
{
    if (!instance)
        instance = new ComponentManager();
    return instance;
}

ComponentManager::ComponentManager() : m_jsObj(NULL)
{
    m_components.clear();
}

ComponentManager::~ComponentManager()
{
    m_components.clear();
}

static void
StartElement(void *userData, const char *name, const char **atts)
{
	Component *component = (Component *) userData;
    AttributeList *attrlist = NULL;
    Object *obj = NULL;
    int i;

	if (!PL_strcasecmp(name, "component")) {
        attrlist = dynamic_cast<AttributeList *>(component);
	}
	else if (!PL_strcasecmp(name, "class")) { 
        obj = new Object();
        attrlist = dynamic_cast<AttributeList *>(obj);
        component->SetCurClass(obj);
	}
	else if (!PL_strcasecmp(name, "function")) { 
        Function *func = new Function();
        attrlist = dynamic_cast<AttributeList *>(func);
        component->GetCurClass()->AddFunction(func);
        component->GetCurClass()->SetCurFunction(func);
	}
	else if (!PL_strcasecmp(name, "arg")) { 
        Arg *arg = new Arg();
        attrlist = dynamic_cast<AttributeList *>(arg);
        component->GetCurClass()->GetCurFunction()->AddArgument(arg);
	}
	else if (!PL_strcasecmp(name, "return")) { 
        Return *ret = new Return();
        attrlist = dynamic_cast<AttributeList *>(ret);
        component->GetCurClass()->GetCurFunction()->SetReturn(ret);
	}
	else if (!PL_strcasecmp(name, "property")) { 
        Property *prop = new Property();
        attrlist = dynamic_cast<AttributeList *>(prop);
        component->GetCurClass()->AddProperty(prop);
	}

    if (attrlist) {
        for (i = 0; atts[i]; i += 2) {
            attrlist->AddAttribute(string(atts[i]), string(atts[i+1]));
        }
    }
    
    // XXX needed the attribute list before we could add the class to 
    // the component, so that the GUUID (id attr) can be added to a 
    // map for later retrieval

    if (obj)
        component->AddClass(obj);
}

static void
EndElement(void *userData, const char *name)
{
}

PRStatus
ComponentManager::ProcessComponents()
{
    // open the components directory
    // read and parse every SIL file

    //__asm int 3;
    char *cwd = PL_getcwd(NULL, 0);
    string componentdir("components"); 

    string path(cwd);
    free(cwd);

    path += "/";
    path += componentdir.c_str();

    PRDir *dir = PR_OpenDir(path.c_str());

    if (dir) {
        PRDirEntry *dirEnt;

        while (1) {

            dirEnt = PR_ReadDir(dir, (PRDirFlags)(PR_SKIP_BOTH | PR_SKIP_HIDDEN));
            if (!dirEnt)
                break;

            string file(path);
            file += "/";
            file += dirEnt->name;
            if (file.find(".sil"))
                ProcessFile(file.c_str(), true);
        }
    } 

    return PR_SUCCESS;
}

PRStatus
ComponentManager::ParseSIL(const string& path, const string& proxyHeaderOut,
    const string& proxySourceOut, const string& componentHeaderOut)
{
    Component *component;

    component = ProcessFile(path, false);

    component->CreateProxyHeader(proxyHeaderOut);
    component->CreateProxySource(proxyHeaderOut, proxySourceOut,
        componentHeaderOut);
    component->CreateComponentHeader(componentHeaderOut);

    return PR_SUCCESS;
}

Component *
ComponentManager::ProcessFile(const string& path, bool live)
{
    int n;
    if ((n = path.find(".sil")) == -1) 
        return NULL;

  	ifstream from(path.c_str());

    if (!from.is_open())
        return NULL;   

    Component *component = new Component(path);
	
    if (!component)
        return NULL;

    char buf[BUFSIZ];
    m_parser = XML_ParserCreate(NULL);
    if (!m_parser) {
        delete component; 
        return NULL;
    }
    PRInt32 done = 0;
    XML_SetUserData(m_parser, component);
    XML_SetElementHandler(m_parser, StartElement, EndElement);
    XML_SetParamEntityParsing(m_parser, XML_PARAM_ENTITY_PARSING_ALWAYS);

    while (!from.eof()) {
        from.getline(buf, BUFSIZ);
        done = from.eof();
        if (XML_Parse(m_parser, buf, strlen(buf), done) == XML_STATUS_ERROR) {
            fprintf(stderr,       // XXX
                "%s at line %d\n",
                XML_ErrorString(XML_GetErrorCode(m_parser)),
                XML_GetCurrentLineNumber(m_parser));
            delete component;
            XML_ParserFree(m_parser);      
            return NULL;
        }
    }
    m_components[component->GetGUUID()] = component;
    if (live)
        component->CreateJSObject();
    XML_ParserFree(m_parser);      
	return component;
}

Component *
ComponentManager::GetComponentByGUUID(const string& guuid)
{
    return  m_components[guuid];
}

// JS support for component manager

JS_STATIC_DLL_CALLBACK(JSBool)
GetComponent(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSString *str;
    JSBool ret = JS_TRUE;

    if (!argc)
        return JS_TRUE;

    str = JS_ValueToString(cx, argv[0]);
    if (!str)
        return JS_FALSE;

    char *bytes = JS_GetStringBytes(str);
    bytes = strdup(bytes);
    if (!bytes)
        return JS_FALSE;

    // argv[0] has the GUUID of the component

    // if the component is already loaded, then just return its object

    ComponentManager *manager = ComponentManager::GetInstance();

    Component *comp; 

    comp = manager->GetComponentByGUUID(string(bytes));
    if (comp != (Component *) NULL) {

        if (!comp->GetIsLoaded()) {
            manager->LoadComponent(comp);
        }
        
        if (comp->GetIsLoaded()) {
            // set the return value to the component's object

            *rval = OBJECT_TO_JSVAL(comp->GetJSObject());
        } else {
            ret = JS_FALSE;
        }
    } else {
        ret = JS_FALSE;
    }

    if (bytes)
        free(bytes);
    return ret;
}

PRStatus
ComponentManager::LoadComponent(Component *comp)
{
    PRStatus ret = PR_FAILURE;

    // load the proxy and implementation libs

    string path(comp->GetPath());

    Library *lib = new Library();

    if (lib->Load(path) == PR_SUCCESS) {
        comp->SetLibrary(lib);
          
/* 
        path = comp->GetProxyPath(); 
        lib = new Library();
        if (lib->Load(path) == PR_SUCCESS) {
            comp->SetProxyLibrary(lib);
*/
            comp->SetIsLoaded(true); 
            ret = PR_SUCCESS;
/*
        }
*/
    }
    return ret;
}

static JSFunctionSpec componentmgr_functions[] = {
    {"getComponent", GetComponent,  1, 0, 0},
    {0, 0, 0, 0, 0}
};

PRStatus
ComponentManager::CreateJSObject()
{
    JSEngine *js = JSEngine::GetJSEngine();

    if (!js)
        return PR_FAILURE;

    JSContext *ctx = js->GetGlobalContext(); 
    JSObject *obj = js->GetGlobalObject();

    static JSClass componentmgr_class = {
        "componentmgr", 0,
        JS_PropertyStub,  JS_PropertyStub,
        JS_PropertyStub,  JS_PropertyStub,
        JS_EnumerateStub,  JS_ResolveStub, 
        JS_ConvertStub,   JS_FinalizeStub
    };        

    JSAutoRequest r(ctx);
    m_jsObj = JS_DefineObject(ctx, obj, "componentmgr", &componentmgr_class, NULL, 0);

    JS_DefineFunctions(ctx, m_jsObj, componentmgr_functions);

    return PR_SUCCESS;
}

JSObject *
ComponentManager::GetJSObject()
{
    return m_jsObj;
}

