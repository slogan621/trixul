/* This is generated source code. Do not edit */

#if !defined(__79463AD7_02C3_42FD_A8F0_061525C45FF8_PROXY_H__)
#define __79463AD7_02C3_42FD_A8F0_061525C45FF8_PROXY_H__

#if defined(WIN32)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include <prtypes.h>
#include "jsapi.h"

extern "C" EXPORT JSBool
AESEncrypt_Get_key(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" EXPORT JSBool
AESEncrypt_Set_key(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" EXPORT JSBool
AESEncrypt_SetKey(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" EXPORT JSBool
AESEncrypt_Encrypt(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" EXPORT JSBool
AESEncrypt_Decrypt(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" EXPORT PRStatus NewAESEncryptProxy(JSObject *obj);

extern "C" EXPORT PRStatus DeleteAESEncryptProxy(JSObject *obj);

#endif
