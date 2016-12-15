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

#include "windowscheckboximpl.h"

WindowsCheckBoxImpl::WindowsCheckBoxImpl() : m_checkbox(NULL)
{
}

WindowsCheckBoxImpl::~WindowsCheckBoxImpl()
{
}

PRStatus WindowsCheckBoxImpl::Create()
{

// XXX see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vcmxspec/html/vcManagedExtensionsSpec_16_3.asp regarding freeing m_checkbox

    m_checkbox = __gc new System::Windows::Forms::CheckBox();
    if (m_checkbox) {
        //m_checkbox->AutoSize = true;
        if (!m_formParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetWidgetParent(top);
            }
        }

        bool added = false;
        if (m_formParent) {
            m_formParent->Controls->Add(m_checkbox);
            added = true;
        } else if (m_svParent) {
            m_svParent->Controls->Add(m_checkbox);
            added = true;
        }
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus WindowsCheckBoxImpl::SetLabel(const string& label)
{
    if (m_checkbox) 
        m_checkbox->Text = label.c_str();
    return PR_SUCCESS;
}

PRStatus 
WindowsCheckBoxImpl::GetChecked(bool &checked)
{
    PRStatus ret = PR_FAILURE;
    if (m_checkbox) {
        checked = m_checkbox->Checked;
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus 
WindowsCheckBoxImpl::SetChecked(const bool checked)
{
    PRStatus ret = PR_FAILURE;
    if (m_checkbox) {
        m_checkbox->Checked = checked;
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus
WindowsCheckBoxImpl::Show()
{
    if (m_checkbox) {
        m_checkbox->Show();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsCheckBoxImpl::Hide()
{
    if (m_checkbox) {
        m_checkbox->Hide();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsCheckBoxImpl::Enable()
{
    if (m_checkbox) {
        m_checkbox->Enabled = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsCheckBoxImpl::Disable()
{
    if (m_checkbox) {
        m_checkbox->Enabled = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus 
WindowsCheckBoxImpl::SetGeometry(const int &x, const int &y,
        const int &width, const int &height, const char &mask)
{
    if (m_checkbox) {
        Size size;
        Point point;

        size = m_checkbox->ClientSize;
        point = m_checkbox->Location;

        if (mask & GEOM_X)
            point.X = x;
        if (mask & GEOM_Y)
            point.Y = y;
        if (mask & GEOM_WIDTH)
            size.Width = width;
        if (mask & GEOM_HEIGHT)
            size.Height = height;

        m_checkbox->ClientSize=size;
        m_checkbox->Location=point;
    } 

    return PR_SUCCESS;
}

PRStatus 
WindowsCheckBoxImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    Size size, request(0, 0);
    

    if (m_checkbox) {
        size = m_checkbox->ClientSize;
        width = size.get_Width();
        height = size.get_Height();
    }
    return PR_SUCCESS;
}
