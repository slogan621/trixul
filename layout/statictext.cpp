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

#include "statictext.h"

StaticText::StaticText() 
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_text = factory->MakeStaticText();
    SetContentType(ContentTypeString);
}

StaticText::StaticText(string& label)
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_text = factory->MakeStaticText();
	SetString(label);
}

StaticText::~StaticText()
{
    if (m_text)
        delete m_text;
}

PRStatus StaticText::SetString(string& value)
{
	AddAttribute(string("string"), value);

	if (m_text)
		m_text->SetString(value);
	return PR_SUCCESS;
}

string& StaticText::GetString()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("string")); 
	if (attribute) 
		return attribute->GetValue();
	return failret;
}

PRStatus StaticText::GetValue(XPVariant &v)
{
    return PR_FAILURE;
}

PRStatus StaticText::SetValue(const XPVariant &v)
{
    string value;
    const_cast<XPVariant &>(v).GetValue(value);
    return SetString(value);
}

PRStatus StaticText::Draw()
{
	return PR_SUCCESS;
}

PRStatus StaticText::Show()
{
    if (m_text)
	    m_text->Show();
    return PR_SUCCESS;
}

PRStatus StaticText::Hide()
{
    if (m_text)
	    m_text->Hide();
    return PR_SUCCESS;
}

PRStatus StaticText::Enable()
{
    if (m_text)
	    m_text->Enable();
    SetEnabled(true);
    return PR_SUCCESS;
}

PRStatus StaticText::Disable()
{
    if (m_text)
	    m_text->Disable();
    SetEnabled(false);
    return PR_SUCCESS;
}

PRStatus StaticText::Create()
{    
    PRStatus status;

    status = m_text->Create();
    if (status == PR_SUCCESS) {
        // set the initial attributes, XXX should come from a table

        status = SetString(GetString());
    }
    return status;
}

PRStatus StaticText::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_text)
        m_text->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}

PRStatus StaticText::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char &mask)
{
    if (m_text)
        m_text->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus StaticText::GetGeometryRequestImpl(int &x, int &y, int &width, int &height)
{
    if (m_text)
        m_text->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}
