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

#include "windowsradiobuttonimpl.h"

WindowsRadioButtonImpl::WindowsRadioButtonImpl() : m_radiobutton(NULL)
{
}

WindowsRadioButtonImpl::~WindowsRadioButtonImpl()
{
}

PRStatus WindowsRadioButtonImpl::HandleCommand()
{
    if (m_radiobutton) {
        SetChecked(true);
        RadioGroupManager *mgr = RadioGroupManager::GetManager(GetDocument());
        if (mgr)
            mgr->SetState(m_group, this);
    }
    return PR_SUCCESS;
}


PRStatus WindowsRadioButtonImpl::Create()
{

// XXX see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vcmxspec/html/vcManagedExtensionsSpec_16_3.asp regarding freeing m_radiobutton

    m_radiobutton = __gc new System::Windows::Forms::RadioButton();
    if (m_radiobutton) {
        m_radiobutton->AutoCheck = false; // we take control of the state
        if (!m_formParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetWidgetParent(top);
            }
        }

        bool added = false;
        if (m_formParent) {
            m_formParent->Controls->Add(m_radiobutton);
            added = true;
        } else if (m_svParent) {
            m_svParent->Controls->Add(m_radiobutton);
            added = true;
        }
        if (added) {
            ButtonCallbackHelper *pHelper;
            pHelper = m_buttonHelper = __gc new ButtonCallbackHelper(this);
            m_radiobutton->add_Click(new 
                EventHandler(pHelper,&ButtonCallbackHelper::OnButtonClick));
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus WindowsRadioButtonImpl::SetLabel(const string& label)
{
    if (m_radiobutton)
        m_radiobutton->Text = label.c_str();
    return PR_SUCCESS;
}

PRStatus 
WindowsRadioButtonImpl::SetGroup(const string &group)
{
    PRStatus ret = PR_SUCCESS;
    m_group = group;
    RadioGroupManager *mgr = RadioGroupManager::GetManager(GetDocument());
    if (mgr)
        mgr->AddToGroup(m_group, this);
    return ret;
}

PRStatus 
WindowsRadioButtonImpl::GetChecked(bool &checked)
{
    PRStatus ret = PR_FAILURE;
    if (m_radiobutton) {
        checked = m_radiobutton->Checked;
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus 
WindowsRadioButtonImpl::SetChecked(const bool checked)
{
    PRStatus ret = PR_FAILURE;
    if (m_radiobutton) {
        m_radiobutton->Checked = checked;
        if (checked == true) {
            RadioGroupManager *mgr = RadioGroupManager::GetManager(GetDocument());
            if (mgr)
                mgr->SetState(m_group, this);
        }
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus
WindowsRadioButtonImpl::Show()
{
    if (m_radiobutton) {
        m_radiobutton->Show();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsRadioButtonImpl::Hide()
{
    if (m_radiobutton) {
        m_radiobutton->Hide();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsRadioButtonImpl::Enable()
{
    if (m_radiobutton) {
        m_radiobutton->Enabled = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsRadioButtonImpl::Disable()
{
    if (m_radiobutton) {
        m_radiobutton->Enabled = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus 
WindowsRadioButtonImpl::SetGeometry(const int &x, const int &y,
        const int &width, const int &height, const char &mask)
{
    if (m_radiobutton) {
        Size size;
        Point point;

        size = m_radiobutton->ClientSize;
        point = m_radiobutton->Location;

        if (mask & GEOM_X)
            point.X = x;
        if (mask & GEOM_Y)
            point.Y = y;
        if (mask & GEOM_WIDTH)
            size.Width = width;
        if (mask & GEOM_HEIGHT)
            size.Height = height;

        m_radiobutton->ClientSize=size;
        m_radiobutton->Location=point;
    } 

    return PR_SUCCESS;
}

PRStatus 
WindowsRadioButtonImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    Size size, request(0, 0);
    

    if (m_radiobutton) {
        size = m_radiobutton->ClientSize;
        width = size.get_Width();
        height = size.get_Height();
    }
    return PR_SUCCESS;
}
