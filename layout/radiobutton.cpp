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

#include "radiobutton.h"
#include "plstr.h"

RadioButton::RadioButton() 
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_radiobutton = factory->MakeRadioButton();
    SetContentType(ContentTypeBoolean);
}

RadioButton::RadioButton(string& label)
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_radiobutton = factory->MakeRadioButton();
	SetLabel(label);
}

RadioButton::~RadioButton()
{
    if (m_radiobutton)
        delete m_radiobutton;
}

PRStatus RadioButton::SetChecked(string &checked)
{
    AddAttribute(string("checked"), checked);

    bool isChecked = false;
    if (!PL_strcasecmp(checked.c_str(), "true"))
        isChecked = true;
    if (m_radiobutton)
        m_radiobutton->SetChecked(isChecked);
    return PR_SUCCESS;
}

string& RadioButton::GetChecked()
{
    AnAttribute *attribute;
    static string failret("");

    attribute = GetAttributeByName(string("checked"));
    if ( attribute )
        return attribute->GetValue();
    return failret;
}

PRStatus RadioButton::SetLabel(string& label)
{
	AddAttribute(string("label"), label);

	if (m_radiobutton)
		m_radiobutton->SetLabel(label);
	return PR_SUCCESS;
}

PRStatus RadioButton::SetGroup(string& group)
{
	AddAttribute(string("group"), group);

	if (m_radiobutton)
		m_radiobutton->SetGroup(group);
	return PR_SUCCESS;
}

PRStatus RadioButton::GetValue(XPVariant &v)
{
    bool value;
    if (m_radiobutton && m_radiobutton->GetChecked(value) == PR_SUCCESS) {
        v.SetValue(value);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus RadioButton::SetValue(const XPVariant &v)
{
    bool value;
    const_cast<XPVariant &>(v).GetValue(value);
    if (m_radiobutton)
        return m_radiobutton->SetChecked(value);
    return PR_FAILURE;
}

string& RadioButton::GetLabel()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("label")); 
	if (attribute) 
		return attribute->GetValue();
	return failret;
}

string& RadioButton::GetGroup()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("group")); 
	if (attribute) 
		return attribute->GetValue();
	return failret;
}

PRStatus RadioButton::Draw()
{
	return PR_SUCCESS;
}

PRStatus RadioButton::Show()
{
    if (m_radiobutton)
	    m_radiobutton->Show();
    return PR_SUCCESS;
}

PRStatus RadioButton::Hide()
{
    if (m_radiobutton)
	    m_radiobutton->Hide();
    return PR_SUCCESS;
}

PRStatus RadioButton::Enable()
{
    if (m_radiobutton)
	    m_radiobutton->Enable();
    SetEnabled(true);
    return PR_SUCCESS;
}

PRStatus RadioButton::Disable()
{
    if (m_radiobutton)
	    m_radiobutton->Disable();
    SetEnabled(false);
    return PR_SUCCESS;
}

PRStatus RadioButton::Create()
{    
    PRStatus status;

    status = m_radiobutton->Create();
    if (status == PR_SUCCESS) {
        status = SetLabel(GetLabel());
        status = SetGroup(GetGroup());
        status = SetChecked(GetChecked());
    }
    return status;
}

PRStatus RadioButton::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_radiobutton)
        m_radiobutton->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}

PRStatus RadioButton::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char& mask)
{
    if (m_radiobutton)
        m_radiobutton->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus RadioButton::GetGeometryRequestImpl(int &x, int &y, int &width, int &height)
{
    if (m_radiobutton)
        m_radiobutton->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}
