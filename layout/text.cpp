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

#include "text.h"

Text::Text() 
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_text = factory->MakeText();
    SetContentType(ContentTypeString);
}

Text::Text(string& label)
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_text = factory->MakeText();
	SetString(label);
}

Text::~Text()
{
    if (m_text)
        delete m_text;
}

void
Text::Init()
{
    AnAttribute *attribute;
    attribute = GetAttributeByName(string("height"));
    if (attribute) 
        m_text->SetHasHeightAttr(true);
    else
        m_text->SetHasHeightAttr(false);
}

PRStatus Text::SetString(string& value)
{
	AddAttribute(string("string"), value);

	if (m_text)
		m_text->SetString(value);
	return PR_SUCCESS;
}

string& Text::GetString()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("string")); 
	if (attribute) 
		return attribute->GetValue();
	return failret;
}

PRStatus Text::GetValue(XPVariant &v)
{
    string value;
    if (m_text && m_text->GetValue(value) == PR_SUCCESS) {
        v.SetValue(value);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus Text::SetValue(const XPVariant &v)
{
    string value;
    const_cast<XPVariant &>(v).GetValue(value);
    return SetString(value);
}

PRStatus Text::SetIsEditable(bool isEditable)
{
	AddAttribute(string("isEditable"), isEditable);

	if (m_text)
		m_text->SetIsEditable(isEditable);
	return PR_SUCCESS;
}

bool Text::GetIsEditable()
{
	AnAttribute *attribute;

	attribute = GetAttributeByName(string("editable")); 
	if (attribute) 
		return attribute->GetValueAsBool();
	return false;
}

PRStatus Text::SetIsSelectable(bool isSelectable)
{
	AddAttribute(string("isSelectable"), isSelectable);

	if (m_text)
		m_text->SetIsSelectable(isSelectable);
	return PR_SUCCESS;
}

bool Text::GetIsSelectable()
{
	AnAttribute *attribute;

	attribute = GetAttributeByName(string("selectable")); 
	if (attribute) 
		return attribute->GetValueAsBool();
	return false;
}

PRStatus Text::Draw()
{
	return PR_SUCCESS;
}

PRStatus Text::Enable()
{
    if (m_text)
	    m_text->Enable();
    SetEnabled(true);
    return PR_SUCCESS;
}

PRStatus Text::Disable()
{
    if (m_text)
	    m_text->Disable();
    SetEnabled(false);
    return PR_SUCCESS;
}

PRStatus Text::Show()
{
    if (m_text)
	    m_text->Show();
    return PR_SUCCESS;
}

PRStatus Text::Hide()
{
    if (m_text)
	    m_text->Hide();
    return PR_SUCCESS;
}

PRStatus Text::Create()
{    
    PRStatus status;

    Init();
    status = m_text->Create();
    if (status == PR_SUCCESS) {
        // set the initial attributes, XXX should come from a table

        status = SetString(GetString());

        m_text->SetIsEditable(GetIsEditable());
        m_text->SetIsSelectable(GetIsSelectable());
    }
    return status;
}

PRStatus Text::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_text)
        m_text->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}

PRStatus Text::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char &mask)
{
    if (m_text)
        m_text->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus Text::GetGeometryRequestImpl(int &x, int &y, int &width, int &height)
{
    if (m_text)
        m_text->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}

