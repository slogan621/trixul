/* This is generated source code. Do not edit */

#include "simple_proxy.h"
#include "simple.h"
#include "prtypes.h"
#include "jsstr.h"
#include <map>

using namespace std;

static map<JSObject *, HelloClass*> objHelloClassMap;

static HelloClass *
GetHelloClassClass(JSObject *obj)
{
    return objHelloClassMap[obj];
}

static void
AddHelloClassClass(JSObject *obj, HelloClass *cPtr)
{
    objHelloClassMap[obj] = cPtr;
}

static void
RemoveHelloClassClass(JSObject *obj)
{
    objHelloClassMap.erase(obj);
}

extern "C" __declspec( dllexport ) PRStatus
NewHelloClassProxy(JSObject *obj)
{
    HelloClass *ptr = new HelloClass();
    if (!ptr)
        return PR_FAILURE;
    AddHelloClassClass(obj, ptr);
    return PR_SUCCESS;
}

extern "C" __declspec( dllexport ) PRStatus
DeleteHelloClassProxy(JSObject *obj)
{
    HelloClass *ptr;
    if (obj) {
        ptr = GetHelloClassClass(obj);
        RemoveHelloClassClass(obj);
        delete ptr;
    }
    return PR_SUCCESS;
}

extern "C" __declspec( dllexport ) JSBool
HelloClass_Hello(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    HelloClass *ptr = GetHelloClassClass(obj);
    if (!ptr)
        return JS_FALSE;
    JSString *str_0 = JSVAL_TO_STRING(argv[0]);
    string arg_0(JS_GetStringBytes(str_0));
    PRInt32 ret;
    ret = ptr->Hello(arg_0);
    *rval = INT_TO_JSVAL(ret);
    return JS_TRUE;
}

extern "C" __declspec( dllexport ) JSBool
HelloClass_Get_age(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    PRInt32 arg_0;
    HelloClass *ptr = GetHelloClassClass(obj);
    if (!ptr || ptr->Get_age(arg_0) != PR_SUCCESS)
        return JS_FALSE;
    *rval = JSVAL_TO_INT(arg_0);
    return JS_TRUE;
}

extern "C" __declspec( dllexport ) JSBool
HelloClass_Set_age(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    PRInt32 arg_0 = JSVAL_TO_INT(argv[0]);
    HelloClass *ptr = GetHelloClassClass(obj);
    if (!ptr || ptr->Set_age(arg_0) != PR_SUCCESS)
        return JS_FALSE;
    return JS_TRUE;
}

static map<JSObject *, HelloClass2*> objHelloClass2Map;

static HelloClass2 *
GetHelloClass2Class(JSObject *obj)
{
    return objHelloClass2Map[obj];
}

static void
AddHelloClass2Class(JSObject *obj, HelloClass2 *cPtr)
{
    objHelloClass2Map[obj] = cPtr;
}

static void
RemoveHelloClass2Class(JSObject *obj)
{
    objHelloClass2Map.erase(obj);
}

extern "C" __declspec( dllexport ) PRStatus
NewHelloClass2Proxy(JSObject *obj)
{
    HelloClass2 *ptr = new HelloClass2();
    if (!ptr)
        return PR_FAILURE;
    AddHelloClass2Class(obj, ptr);
    return PR_SUCCESS;
}

extern "C" __declspec( dllexport ) PRStatus
DeleteHelloClass2Proxy(JSObject *obj)
{
    HelloClass2 *ptr;
    if (obj) {
        ptr = GetHelloClass2Class(obj);
        RemoveHelloClass2Class(obj);
        delete ptr;
    }
    return PR_SUCCESS;
}

extern "C" __declspec( dllexport ) JSBool
HelloClass2_Hello(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    HelloClass2 *ptr = GetHelloClass2Class(obj);
    if (!ptr)
        return JS_FALSE;
    JSString *str_0 = JSVAL_TO_STRING(argv[0]);
    string arg_0(JS_GetStringBytes(str_0));
    PRInt32 ret;
    ret = ptr->Hello(arg_0);
    *rval = INT_TO_JSVAL(ret);
    return JS_TRUE;
}

extern "C" __declspec( dllexport ) JSBool
HelloClass2_Get_age(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    PRInt32 arg_0;
    HelloClass2 *ptr = GetHelloClass2Class(obj);
    if (!ptr || ptr->Get_age(arg_0) != PR_SUCCESS)
        return JS_FALSE;
    *rval = JSVAL_TO_INT(arg_0);
    return JS_TRUE;
}

extern "C" __declspec( dllexport ) JSBool
HelloClass2_Set_age(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    PRInt32 arg_0 = JSVAL_TO_INT(argv[0]);
    HelloClass2 *ptr = GetHelloClass2Class(obj);
    if (!ptr || ptr->Set_age(arg_0) != PR_SUCCESS)
        return JS_FALSE;
    return JS_TRUE;
}

