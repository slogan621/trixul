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

#include "button.h"

#include "jsengine.h"

Button::Button() 
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_button = factory->MakeButton();
    RegisterListener();
}

Button::Button(string& label)
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_button = factory->MakeButton();
	SetLabel(label);
    RegisterListener();
}

Button::~Button()
{
    UnRegisterListener();
    if (m_button)
        delete m_button;
}

PRStatus Button::SetLabel(string& label)
{
	AddAttribute(string("label"), label);

	if (m_button)
		m_button->SetLabel(label);
	return PR_SUCCESS;
}

PRStatus Button::GetValue(XPVariant &v)
{
    return PR_FAILURE;
}

PRStatus Button::SetValue(const XPVariant &v)
{
    return PR_FAILURE;
}

string& Button::GetLabel()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("label")); 
	if ( attribute ) 
		return attribute->GetValue();
	return failret;
}

PRStatus Button::Draw()
{
	return PR_SUCCESS;
}

PRStatus Button::Show()
{
    if (m_button)
	    m_button->Show();
    return PR_SUCCESS;
}

PRStatus Button::Hide()
{
    if (m_button)
	    m_button->Hide();
    return PR_SUCCESS;
}

PRStatus Button::Enable()
{
    if (m_button)
	    m_button->Enable();
    SetEnabled(true);
    return PR_SUCCESS;
}

PRStatus Button::Disable()
{
    if (m_button)
	    m_button->Disable();
    SetEnabled(false);
    return PR_SUCCESS;
}

PRStatus Button::Create()
{    
    PRStatus status;

    status = m_button->Create();
    if (status == PR_SUCCESS) {
        status = SetLabel(GetLabel());
    }
    return status;
}

void Button::ButtonPressed()
{
	AnAttribute *attribute;
    string val;
    JSObject *obj;
    JSContext *cx;

    Document *doc = GetDocument();

    if (doc) {
        obj = doc->GetJSObject();
        cx = doc->GetJSCtx();
    }

	attribute = GetAttributeByName(string("onclick")); 
	if (attribute)  {
		val = attribute->GetValue();
        if (val.size()) {
            JS_BeginRequest(cx);
            JSFunction *func = JS_CompileFunction(cx, obj, "onclick",
                0, NULL, val.c_str(), val.size(), "internal", 1);
            JS_EndRequest(cx);
            if (func) {
                jsval rval;
                JSBool ret;
                JS_BeginRequest(cx);
                ret = JS_CallFunction(cx, obj, func, 0, NULL, &rval);
                JS_EndRequest(cx);
                if (ret == JS_TRUE) {
                    printf("Button::ButtonPressed success\n");
                    if (JSVAL_IS_OBJECT(rval)) {
                        printf("Button::ButtonPressed. return is an object\n");
                    }
                    if (JSVAL_IS_NUMBER(rval)) {
                        printf("Button::ButtonPressed. return is a number\n");
                    }
                    if (JSVAL_IS_INT(rval)) {
                        printf("Button::ButtonPressed. return is an int\n");
                    }
                    if (JSVAL_IS_DOUBLE(rval)) {
                        printf("Button::ButtonPressed. return is a double\n");
                    }
                    if (JSVAL_IS_STRING(rval)) {
                        printf("Button::ButtonPressed. return is a string\n");
                    }
                    if (JSVAL_IS_BOOLEAN(rval)) {
                        printf("Button::ButtonPressed. return is a bool\n");
                    }
                    if (JSVAL_IS_NULL(rval)) {
                        printf("Button::ButtonPressed. return is a NULL\n");
                    }
                    if (JSVAL_IS_VOID(rval)) {
                        printf("Button::ButtonPressed. return is an void\n");
                    }
                    if (JSVAL_IS_PRIMITIVE(rval)) {
                        printf("Button::ButtonPressed. return is a primitive\n");
                    }
                    if (JSVAL_IS_INT(rval))
                        printf("Button::ButtonPressed return is %d\n",
                            JSVAL_TO_INT(rval));
                } else {
                    printf("Button::ButtonPressed fail\n");
                }
            }
        }
    }
}

PRStatus
Button::RegisterListener()
{
    ButtonPressSubject *pBPS =
        dynamic_cast <ButtonPressSubject *>(m_button);
    pBPS->Attach(this);
    return PR_SUCCESS;
}

PRStatus
Button::UnRegisterListener()
{
    ButtonPressSubject *pBPS =
        dynamic_cast <ButtonPressSubject *>(m_button);
    pBPS->Detach(this);
    return PR_SUCCESS;
}

PRStatus Button::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char& mask)
{
    if (m_button)
        m_button->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus Button::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_button)
        m_button->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}

PRStatus Button::GetGeometryRequestImpl(int &x, int &y, int &width, int &height)
{
    if (m_button)
        m_button->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}
