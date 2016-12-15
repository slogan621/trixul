/* This is generated source code. Do not edit */

#if !defined(__94981D9E_FC99_11D9_8BDE_F66BAD1E3F3A_PROXY_H__)
#define __94981D9E_FC99_11D9_8BDE_F66BAD1E3F3A_PROXY_H__

#include <prtypes.h>
#include "jsapi.h"

extern "C" __declspec( dllexport ) JSBool
HelloClass_Get_age(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" __declspec( dllexport ) JSBool
HelloClass_Set_age(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" __declspec( dllexport ) JSBool
HelloClass_Hello(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" __declspec( dllexport ) PRStatus NewHelloClassProxy(JSObject *obj);

extern "C" __declspec( dllexport ) PRStatus DeleteHelloClassProxy(JSObject *obj);

extern "C" __declspec( dllexport ) JSBool
HelloClass2_Get_age(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" __declspec( dllexport ) JSBool
HelloClass2_Set_age(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" __declspec( dllexport ) JSBool
HelloClass2_Hello(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern "C" __declspec( dllexport ) PRStatus NewHelloClass2Proxy(JSObject *obj);

extern "C" __declspec( dllexport ) PRStatus DeleteHelloClass2Proxy(JSObject *obj);

#endif
