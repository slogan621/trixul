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

#include "filepicker.h"
#include "openpickerimpl.h"
#include "saveaspickerimpl.h"
#include "widgetfactory.h"

static FilePicker *instance = NULL;

FilePicker* 
FilePicker::GetInstance()
{
    if (!instance)
        instance = new FilePicker();
    return instance;
}

FilePicker::FilePicker() : m_jsObj(NULL)
{
}

FilePicker::~FilePicker()
{
}

// JS support for component manager

JS_STATIC_DLL_CALLBACK(JSBool)
DoOpenPicker(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    *rval = STRING_TO_JSVAL("");
    OpenPickerImpl *openPicker;
    WidgetFactory *factory = GetWidgetFactory();
    if (!factory)
        return JS_FALSE;

    openPicker = factory->MakeOpenPicker();
    if (openPicker && openPicker->Create("") == PR_SUCCESS) {
        string files;
        if (openPicker->GetFile("", files) == PR_SUCCESS) {
            JSString *str = ::JS_NewStringCopyN(cx, files.c_str(), 
                files.size() + 1);
            *rval = STRING_TO_JSVAL(str);
        }
    }

    JSBool ret = JS_TRUE;

    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
DoSaveAsPicker(JSContext *cx, JSObject *obj, uintN argc, 
    jsval *argv, jsval *rval)
{
    *rval = STRING_TO_JSVAL("");
    SaveAsPickerImpl *saveAsPicker;
    WidgetFactory *factory = GetWidgetFactory();
    if (!factory)
        return JS_FALSE;

    saveAsPicker = factory->MakeSaveAsPicker();
    if (saveAsPicker && saveAsPicker->Create("") == PR_SUCCESS) {
        string files;
        if (saveAsPicker->GetFile("", files) == PR_SUCCESS) {
            JSString *str = ::JS_NewStringCopyN(cx, files.c_str(), 
                files.size() + 1);
            *rval = STRING_TO_JSVAL(str);
        }
    }

    JSBool ret = JS_TRUE;

    return ret;
}

static JSFunctionSpec filepicker_functions[] = {
    {"openPicker", DoOpenPicker,  1, 0, 0},
    {"saveAsPicker", DoSaveAsPicker,  1, 0, 0},
    {0, 0, 0, 0, 0}
};

PRStatus
FilePicker::CreateJSObject()
{
    JSEngine *js = JSEngine::GetJSEngine();

    if (!js)
        return PR_FAILURE;

    if (m_jsObj)
        return PR_SUCCESS;

    JSContext *ctx = js->GetGlobalContext(); 
    JSObject *obj = js->GetGlobalObject();

    static JSClass filepicker_class = {
        "filepicker", 0,
        JS_PropertyStub,  JS_PropertyStub,
        JS_PropertyStub,  JS_PropertyStub,
        JS_EnumerateStub,  JS_ResolveStub, 
        JS_ConvertStub,   JS_FinalizeStub
    };        

    JSAutoRequest r(ctx);
    m_jsObj = JS_DefineObject(ctx, obj, "filepicker", 
        &filepicker_class, NULL, 0);

    JS_DefineFunctions(ctx, m_jsObj, filepicker_functions);

    return PR_SUCCESS;
}

JSObject *
FilePicker::GetJSObject()
{
    return m_jsObj;
}

