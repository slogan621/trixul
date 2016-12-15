/* This is generated source code. Do not edit */

#include "encrypt_proxy.h"
#include "encrypt.h"
#include "prtypes.h"
#include "jsstr.h"
#include <map>

using namespace std;

static map<JSObject *, AESEncrypt*> objAESEncryptMap;

static AESEncrypt *
GetAESEncryptClass(JSObject *obj)
{
    return objAESEncryptMap[obj];
}

static void
AddAESEncryptClass(JSObject *obj, AESEncrypt *cPtr)
{
    objAESEncryptMap[obj] = cPtr;
}

static void
RemoveAESEncryptClass(JSObject *obj)
{
    objAESEncryptMap.erase(obj);
}

extern "C" EXPORT PRStatus
NewAESEncryptProxy(JSObject *obj)
{
    AESEncrypt *ptr = new AESEncrypt();
    if (!ptr)
        return PR_FAILURE;
    AddAESEncryptClass(obj, ptr);
    return PR_SUCCESS;
}

extern "C" EXPORT PRStatus
DeleteAESEncryptProxy(JSObject *obj)
{
    AESEncrypt *ptr;
    if (obj) {
        ptr = GetAESEncryptClass(obj);
        RemoveAESEncryptClass(obj);
        delete ptr;
    }
    return PR_SUCCESS;
}

extern "C" EXPORT JSBool
AESEncrypt_SetKey(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    AESEncrypt *ptr = GetAESEncryptClass(obj);
    if (!ptr)
        return JS_FALSE;
    JSString *str_0 = JSVAL_TO_STRING(argv[0]);
    string arg_0(JS_GetStringBytes(str_0));
    ptr->SetKey(arg_0);
    return JS_TRUE;
}

extern "C" EXPORT JSBool
AESEncrypt_Encrypt(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    AESEncrypt *ptr = GetAESEncryptClass(obj);
    if (!ptr)
        return JS_FALSE;
    JSString *str_0 = JSVAL_TO_STRING(argv[0]);
    string arg_0(JS_GetStringBytes(str_0));
    string ret;
    ret = ptr->Encrypt(arg_0);
    JSString *str = ::JS_NewStringCopyN(cx, ret.c_str(), ret.size() + 1);
    *rval = STRING_TO_JSVAL(str);
    return JS_TRUE;
}

extern "C" EXPORT JSBool
AESEncrypt_Decrypt(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    AESEncrypt *ptr = GetAESEncryptClass(obj);
    if (!ptr)
        return JS_FALSE;
    JSString *str_0 = JSVAL_TO_STRING(argv[0]);
    string arg_0(JS_GetStringBytes(str_0));
    string ret;
    ret = ptr->Decrypt(arg_0);
    JSString *str = ::JS_NewStringCopyN(cx, ret.c_str(), ret.size() + 1);
    *rval = STRING_TO_JSVAL(str);
    return JS_TRUE;
}

extern "C" EXPORT JSBool
AESEncrypt_Get_key(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    string arg_0;
    AESEncrypt *ptr = GetAESEncryptClass(obj);
    if (!ptr || ptr->Get_key(arg_0) != PR_SUCCESS)
        return JS_FALSE;
    JSString *str = ::JS_NewStringCopyN(cx, arg_0.c_str(), arg_0.size() + 1);
    *rval = STRING_TO_JSVAL(str);
    return JS_TRUE;
}

extern "C" EXPORT JSBool
AESEncrypt_Set_key(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSString *str_0 = JSVAL_TO_STRING(argv[0]);
    string arg_0(JS_GetStringBytes(str_0));
    AESEncrypt *ptr = GetAESEncryptClass(obj);
    if (!ptr || ptr->Set_key(arg_0) != PR_SUCCESS)
        return JS_FALSE;
    return JS_TRUE;
}

