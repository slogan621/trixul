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

#include "checkbox.h"
#include "plstr.h"

CheckBox::CheckBox() 
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_checkbox = factory->MakeCheckBox();
    SetContentType(ContentTypeBoolean);
}

CheckBox::CheckBox(string& label)
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_checkbox = factory->MakeCheckBox();
	SetLabel(label);
}

CheckBox::~CheckBox()
{
    if (m_checkbox)
        delete m_checkbox;
}

PRStatus CheckBox::SetChecked(string &checked)
{
    AddAttribute(string("checked"), checked);
    
    bool isChecked = false;
    if (!PL_strcasecmp(checked.c_str(), "true"))
        isChecked = true;
    if (m_checkbox)
        m_checkbox->SetChecked(isChecked);
    return PR_SUCCESS;
}

string& CheckBox::GetChecked()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("checked")); 
	if ( attribute ) 
		return attribute->GetValue();
	return failret;
}

PRStatus CheckBox::SetLabel(string& label)
{
	AddAttribute(string("label"), label);

	if (m_checkbox)
		m_checkbox->SetLabel(label);
	return PR_SUCCESS;
}

PRStatus CheckBox::GetValue(XPVariant &v)
{
    bool value;
    if (m_checkbox && m_checkbox->GetChecked(value) == PR_SUCCESS) {
        v.SetValue(value);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus CheckBox::SetValue(const XPVariant &v)
{
    bool value;
    const_cast<XPVariant &>(v).GetValue(value);
    if (m_checkbox)
        return m_checkbox->SetChecked(value);
    return PR_FAILURE;
}

string& CheckBox::GetLabel()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("label")); 
	if ( attribute ) 
		return attribute->GetValue();
	return failret;
}

PRStatus CheckBox::Draw()
{
	return PR_SUCCESS;
}

PRStatus CheckBox::Show()
{
    if (m_checkbox)
	    m_checkbox->Show();
    return PR_SUCCESS;
}

PRStatus CheckBox::Hide()
{
    if (m_checkbox)
	    m_checkbox->Hide();
    return PR_SUCCESS;
}

PRStatus CheckBox::Enable()
{
    if (m_checkbox)
	    m_checkbox->Enable();
    SetEnabled(true);
    return PR_SUCCESS;
}

PRStatus CheckBox::Disable()
{
    if (m_checkbox)
	    m_checkbox->Disable();
    SetEnabled(false);
    return PR_SUCCESS;
}

PRStatus CheckBox::Create()
{    
    PRStatus status;

    status = m_checkbox->Create();
    if (status == PR_SUCCESS) {
        status = SetLabel(GetLabel());
        status = SetChecked(GetChecked());
    }
    return status;
}

PRStatus CheckBox::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_checkbox)
        m_checkbox->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}

PRStatus CheckBox::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char& mask)
{
    if (m_checkbox)
        m_checkbox->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus CheckBox::GetGeometryRequestImpl(int &x, int &y, int &width, int &height)
{
    if (m_checkbox)
        m_checkbox->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}
