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

#include "windowstextimpl.h"

WindowsTextImpl::WindowsTextImpl()
{
}

WindowsTextImpl::~WindowsTextImpl()
{
}

PRStatus WindowsTextImpl::Create()
{
    PRStatus ret = PR_FAILURE;
    m_text = __gc new TextBox();
    if (m_text) {
        m_text->Multiline = false;
        if (!m_formParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetWidgetParent(top);
            }
        }

        // we want to track focus, so register for events

        m_gotFocusHelper = __gc new ControlGotFocusHelper(this);

        m_text->add_GotFocus(new EventHandler(static_cast<ControlGotFocusHelper *>(m_gotFocusHelper), &ControlGotFocusHelper::OnGotFocusHandler));

        bool added = false;
        if (m_formParent) {
            m_formParent->Controls->Add(m_text);
            added = true;
        } else if (m_svParent) {
            m_svParent->Controls->Add(m_text);
            added = true;
        }
	    if (added)
	        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus WindowsTextImpl::SetString(const string& value)
{
    if (m_text) {
        m_text->Text = value.c_str();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus WindowsTextImpl::SetIsEditable(const bool isEditable)
{
    if (m_text) {
        m_text->ReadOnly = !isEditable;
	    return PR_SUCCESS;
    }
	return PR_FAILURE;
}

PRStatus WindowsTextImpl::SetIsSelectable(const bool isSelectable)
{
    // not implementable in current version of .NET Forms
    return PR_SUCCESS;
}

PRStatus    
WindowsTextImpl::Enable()
{   
    if (m_text) {
        m_text->Enabled = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus    
WindowsTextImpl::Disable()
{   
    if (m_text) {
        m_text->Enabled = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsTextImpl::GetValue(string &v)
{
    if (m_text) {
        String *str = m_text->Text;
        char *buf;
        buf = reinterpret_cast<char *>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str).ToPointer());
        v = buf; 
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsTextImpl::SetValue(const string &v)
{
    if (m_text) {
        m_text->Text = v.c_str();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus    
WindowsTextImpl::Show()
{   
    if (m_text) {
        m_text->Show();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsTextImpl::Hide()
{
    if (m_text) {
        m_text->Hide();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}       

PRStatus 
WindowsTextImpl::SetGeometry(const int &x, const int &y,
        const int &width, const int &height, const char &mask)
{
    if (m_text) {
        Size size;
        Point point;

        size = m_text->ClientSize;
        point = m_text->Location;

        if (mask & GEOM_X)
            point.X = x;
        if (mask & GEOM_Y)
            point.Y = y;
        if (mask & GEOM_WIDTH)
            size.Width = width;
        if (mask & GEOM_HEIGHT)
            size.Height = height;

        m_text->ClientSize=size;
        m_text->Location=point;
    } 

    return PR_SUCCESS;
}

PRStatus 
WindowsTextImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    Size size, request(0, 0);
    
    if (m_text) {
        size = m_text->ClientSize;
        width = size.get_Width();
        height = m_text->PreferredHeight;
    }
    return PR_SUCCESS;
}

void
WindowsTextImpl::OnCut()
{
    if (m_text)
        m_text->Cut();
}

void
WindowsTextImpl::OnCopy()
{
    if (m_text)
        m_text->Copy();
}

void
WindowsTextImpl::OnPaste()
{
    if (m_text)
        m_text->Paste();
}

void
WindowsTextImpl::OnClear()
{
    if (m_text)
        m_text->Clear();
}

void
WindowsTextImpl::OnSelectAll()
{
    if (m_text)
        m_text->SelectAll();
}


