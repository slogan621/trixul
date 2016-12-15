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

#include "element.h"
#include "control.h"
#include "document.h"
#include "widget.h"

Element::Element() : m_parent(NULL), m_jsObj(NULL)
{
}

Element::Element(Element *parent) : m_jsObj(NULL)
{
	m_parent = parent;
}

void Element::SetType(const ElementType type)
{
    Widget *w = dynamic_cast<Widget *>(this);
    if (w) {
        WidgetImpl *impl = w->GetImpl();
        if (impl)
            impl->SetType(type);
    }
	m_type = type;
}

void Element::AddChild(Element *child, const int level)
{
	child->SetLevel(level);
	m_children.push_back(child);
}

void Element::SetLevel(const int level)
{
	m_level = level;
}

int Element::GetLevel()
{
	return m_level;
}

void Element::SetDocument(Document *doc)
{
	m_doc = doc;
    Widget *w = dynamic_cast<Widget *>(this);
    if (w) {
        WidgetImpl *impl = w->GetImpl();
        if (impl)
            impl->SetDocument(doc);
    }
}

Document *Element::GetDocument()
{
	return m_doc;
}

ElementType Element::GetType()
{
	return m_type;
}

Element *Element::GetParent()
{
	return m_parent;
}

void Element::SetParent(Element *parent)
{
	m_parent = parent;

    // tell the implementation 

#if 0
    Widget *w, *p;
    WidgetImpl *wimpl, *pimpl;
    w = dynamic_cast<Widget *>(this);
    p = dynamic_cast<Widget *>(parent);
    if (w && p) {
        wimpl = w->GetImpl();
        pimpl = p->GetImpl();
        if (wimpl && pimpl) {
            wimpl->SetParent(pimpl);
        }
    }
#endif
}

// JS support for element

PRStatus
Element::GetValueHelper(JSContext *cx, Control *control, jsval *rval)
{
    if (!control)
        return PR_FAILURE;

    PRStatus ret = PR_FAILURE;

    ContentType type = control->GetContentType();
    switch (type) {
    case ContentTypeBoolean:
    {
        XPVariant v;
        if (control->GetValue(v) == PR_SUCCESS) {
            bool val;
            if (v.GetValue(val) == PR_SUCCESS) {
                *rval = BOOLEAN_TO_JSVAL(val);
                ret = PR_SUCCESS;
            }
        }
        break;
    }
    case ContentTypeString:
    {
        XPVariant v;
        if (control->GetValue(v) == PR_SUCCESS) {
            string val;
            if (v.GetValue(val) == PR_SUCCESS) {
                JSString *str = ::JS_NewStringCopyN(cx, val.c_str(), val.size() + 1);
                *rval = STRING_TO_JSVAL(str);
                ret = PR_SUCCESS;
            }
        }
        break;
    }
    default:
        break;
    }
    return ret; 
}

PRStatus
Element::SetValueHelper(JSContext *cx, Control *control, jsval val, jsval *rval)
{
    if (!control)
        return PR_FAILURE;

    PRStatus ret = PR_FAILURE;

    ContentType type = control->GetContentType();
    switch (type) {
    case ContentTypeBoolean:
    {
        JSBool b;
        JS_ValueToBoolean(cx, val, &b);
        XPVariant v;
        v.SetValue(static_cast<bool>(b));
        if (control->SetValue(v) == PR_SUCCESS) 
            ret = PR_SUCCESS;;
        break;
    }
    case ContentTypeString:
    {
        JSString *str;
        str = JS_ValueToString(cx, val);
        if (!str)
            return ret;
        
        char *bytes = JS_GetStringBytes(str);
        if (bytes)
            bytes = strdup(bytes);
        if (bytes) {
            XPVariant v;
            v.SetValue(bytes);
            if (control->SetValue(v) == PR_SUCCESS) 
                ret = PR_SUCCESS;;
            free(bytes);
        }
        break;
    }
    default:
        break;
    }
    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
DoGetValue(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control && element->GetValueHelper(cx, control, rval) == PR_SUCCESS)
            ret = JS_TRUE;
    } else
        printf("In DoGetValue, did not find an element!!!\n");
    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
DoSetValue(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;

    if (!argc)
        return JS_TRUE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control && 
            element->SetValueHelper(cx, control, argv[0], rval) == PR_SUCCESS)
            ret = JS_TRUE;
        else
            printf("In DoSetValue, did not find an element!!!\n");
    }

    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
DoEnable(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;
    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            control->Enable();
            ret = JS_TRUE;
        }
    } else
        printf("In DoEnable, did not find an element!!!\n");


    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
DoDisable(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            control->Disable();
            ret = JS_TRUE;
        }
    } else
        printf("In DoDisable, did not find an element!!!\n");

    return ret;
}

// appendItem(string value)

JS_STATIC_DLL_CALLBACK(JSBool)
DoAppendItem(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            JSString *str;
            str = JS_ValueToString(cx, argv[0]);
            if (!str)
                return ret;

            char *bytes = JS_GetStringBytes(str);
            if (bytes)
                bytes = strdup(bytes);
            if (bytes) {
                string str(bytes);
                if (control->AppendItem(str) == PR_SUCCESS)
                    ret = JS_TRUE;
                free(bytes);
            }
        }
    } else
        printf("In DoAppendItem, did not find an element!!!\n");

    return ret;
}

// addItem(string value, int position)

JS_STATIC_DLL_CALLBACK(JSBool)
DoAddItem(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            JSString *str;
            str = JS_ValueToString(cx, argv[0]);
            if (!str)
                return ret;
            char *bytes = JS_GetStringBytes(str);
            if (bytes)
                bytes = strdup(bytes);
            if (bytes) {
                string str(bytes);
                int position = 0;
                if (control->AddItem(str, position) == PR_SUCCESS)
                    ret = JS_TRUE;
                free(bytes);
            }
        }

    } else
        printf("In DoAddItem, did not find an element!!!\n");

    return ret;
}

// removeItemByPosition(int position)

JS_STATIC_DLL_CALLBACK(JSBool)
DoRemoveItemByPosition(JSContext *cx, JSObject *obj, uintN argc, 
    jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            int position = JSVAL_TO_INT(argv[0]);
            if (control->RemoveItemByPosition(position) == PR_SUCCESS)
                ret = JS_TRUE;
        }
    } else
        printf("In DoRemoveItemByPosition, did not find an element!!!\n");

    return ret;
}

// removeItemByValue(string value)

JS_STATIC_DLL_CALLBACK(JSBool)
DoRemoveItemByValue(JSContext *cx, JSObject *obj, uintN argc, 
    jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            JSString *str;
            str = JS_ValueToString(cx, argv[0]);
            if (!str)
                return ret;
            char *bytes = JS_GetStringBytes(str);
            if (bytes)
                bytes = strdup(bytes);
            if (bytes) {
                string str(bytes);
                if (control->RemoveItemByValue(str) == PR_SUCCESS)
                    ret = JS_TRUE;
                free(bytes);
            }
        }

    } else
        printf("In DoRemoveItemByValue, did not find an element!!!\n");
    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
DoGetSelectionCount(JSContext *cx, JSObject *obj, uintN argc, 
    jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            int count;
            if (control->GetSelectionCount(count) == PR_SUCCESS) {
                *rval = INT_TO_JSVAL(count);
                ret = JS_TRUE;
            }
        }
    } else
        printf("In DoGetSelectionCount, did not find an element!!!\n");
    return ret;
}

// string getSelection(int index);

JS_STATIC_DLL_CALLBACK(JSBool)
DoGetSelection(JSContext *cx, JSObject *obj, uintN argc, 
    jsval *argv, jsval *rval)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            string str;
            int index = JSVAL_TO_INT(argv[0]);
            if (control->GetSelection(index, str) == PR_SUCCESS) {
                JSString *jsStr = ::JS_NewStringCopyN(cx, str.c_str(), 
                    str.size() + 1);       
                *rval = STRING_TO_JSVAL(jsStr);
                ret = JS_TRUE;
            }
        }
    } else
        printf("In DoGetSelection, did not find an element!!!\n");

    return ret;
}

static JSFunctionSpec element_functions[] = {
    {"enable", DoEnable,  1, 0, 0},
    {"disable", DoDisable,  1, 0, 0},
    {"setValue", DoSetValue,  1, 0, 0},
    {"getValue", DoGetValue,  1, 0, 0},
    {"addItem", DoAddItem,  1, 0, 0},
    {"appendItem", DoAppendItem,  1, 0, 0},
    {"removeItemByPosition", DoRemoveItemByPosition,  1, 0, 0},
    {"removeItemByValue", DoRemoveItemByValue,  1, 0, 0},
    {"getSelection", DoGetSelection,  1, 0, 0},
    {"getSelectionCount", DoGetSelectionCount,  1, 0, 0},
    {0, 0, 0, 0, 0}
};

static JSBool
EnabledPropertySetter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JSBool ret = JS_FALSE;
    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            bool enable = JSVAL_TO_BOOLEAN(*vp);
            if (enable)
                control->Enable();
            else
                control->Disable();
            ret = JS_TRUE;
        }
    } else
        printf("In EnabledPropertySetter, did not find an element!!!\n");

    return ret;
}

static JSBool
EnabledPropertyGetter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control) {
            *vp = BOOLEAN_TO_JSVAL(control->GetEnabled());
            ret = JS_TRUE;
        }
    }
    return ret;
}

static JSBool
ValuePropertySetter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
  
        if (control && element->SetValueHelper(cx, control, *vp, vp) == PR_SUCCESS)
            ret = JS_TRUE;
        else
            printf("In ValuePropertySetter, did not find an element!!!\n");
    }

    return ret;
}

static JSBool
ValuePropertyGetter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JSBool ret = JS_FALSE;

    Element *element = Element::FindElementByObj(obj);
    if (element) {
        Control *control = dynamic_cast<Control *>(element);
        if (control && element->GetValueHelper(cx, control, vp) == PR_SUCCESS)
            ret = JS_TRUE;
    } else
        printf("In DoGetValue, did not find an element!!!\n");
    return ret;
}

static JSPropertySpec element_properties[] = {
    {"enabled", 0x01, 0, EnabledPropertyGetter,  EnabledPropertySetter},
    {"value", 0x02, 0, ValuePropertyGetter,  ValuePropertySetter},
    {0}
};

// XXX memory management on this object

PRStatus
Element::CreateJSObject()
{
    JSContext *ctx;
    JSObject *obj;
    if (m_jsObj)
        return PR_SUCCESS;

    Document *doc = GetDocument();

    if (doc) {
        obj = doc->GetJSObject();
        ctx = doc->GetJSCtx();
    }

    static JSClass element_class = {
        "element", 0,
        JS_PropertyStub,  JS_PropertyStub,
        JS_PropertyStub,  JS_PropertyStub,
        JS_EnumerateStub,  JS_ResolveStub,
        JS_ConvertStub,   JS_FinalizeStub
    };

    JSAutoRequest r(ctx);
    m_jsObj = JS_DefineObject(ctx, obj, "element", &element_class, NULL, 0);
    if (m_jsObj) {
        JS_DefineFunctions(ctx, m_jsObj, element_functions);
        JS_DefineProperties(ctx, m_jsObj, element_properties);
        m_objectMap.insert(pair<JSObject *, Element *>(m_jsObj, this));
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

JSObject *
Element::GetJSObject()
{
    return m_jsObj;
}

Element *Element::FindElementByObj(JSObject *obj)
{
    map<JSObject *, Element *>::iterator p;

    p = m_objectMap.find(obj);
    if (p != m_objectMap.end())
    {   
        return p->second;
    }
    return NULL;
}

void Element::Accept(AbstractElementVisitor *elementVisitor) 
{
	elementVisitor->VisitElement(this);
}


void Element::PrintInformation() 
{
   
}

map <JSObject *, Element *> Element::m_objectMap;
