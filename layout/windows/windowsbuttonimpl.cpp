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

#include "windowsbuttonimpl.h"

ButtonCallbackHelper::ButtonCallbackHelper(CommandHandler *impl) 
{
    m_impl = impl;
}

void ButtonCallbackHelper::OnButtonClick(Object *sender, System::EventArgs* e)
{
    if (m_impl) 
        m_impl->HandleCommand();
}

WindowsButtonImpl::WindowsButtonImpl() : m_button(NULL)
{
}

WindowsButtonImpl::~WindowsButtonImpl()
{
}

PRStatus WindowsButtonImpl::HandleCommand()
{
    NotifyButtonPress();
    return PR_SUCCESS;
}

PRStatus WindowsButtonImpl::Create()
{

// XXX see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vcmxspec/html/vcManagedExtensionsSpec_16_3.asp regarding freeing m_button

    m_button = __gc new Button();
    if (m_button) {
        //m_button->AutoSize = true;
        if (!m_formParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetWidgetParent(top);
            }
        }

        bool added = false;
        if (m_formParent) {
            m_formParent->Controls->Add(m_button);
            added = true;
        } else if (m_svParent) {
            m_svParent->Controls->Add(m_button);
            added = true;
        }
        if (added) {
            ButtonCallbackHelper *pHelper;
            pHelper = m_buttonHelper = __gc new ButtonCallbackHelper(this);
            m_button->add_Click(new 
                EventHandler(pHelper,&ButtonCallbackHelper::OnButtonClick));
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus WindowsButtonImpl::SetLabel(const string& label)
{
    if (m_button)
        m_button->Text = label.c_str();
    return PR_SUCCESS;
}

PRStatus
WindowsButtonImpl::Show()
{
    if (m_button) {
        m_button->Show();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsButtonImpl::Hide()
{
    if (m_button) {
        m_button->Hide();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsButtonImpl::Enable()
{
    if (m_button) {
        m_button->Enabled = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsButtonImpl::Disable()
{
    if (m_button) {
        m_button->Enabled = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus 
WindowsButtonImpl::SetGeometry(const int &x, const int &y,
        const int &width, const int &height, const char &mask)
{
    if (m_button) {
        Size size;
        Point point;

        size = m_button->ClientSize;
        point = m_button->Location;

        if (mask & GEOM_X)
            point.X = x;
        if (mask & GEOM_Y)
            point.Y = y;
        if (mask & GEOM_WIDTH)
            size.Width = width;
        if (mask & GEOM_HEIGHT)
            size.Height = height;

        m_button->ClientSize=size;
        m_button->Location=point;
    } 

    return PR_SUCCESS;
}

PRStatus 
WindowsButtonImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    Size size, request(0, 0);
    
    if (m_button) {
        String *label;
        Font *font;
    
        label = m_button->Text;
        font = m_button->Font;
        //size = TextRenderer.MeasureText(font, label); .NET 2.0
        SizeF sizef;
        Graphics *graphics;

        graphics = m_button->CreateGraphics();
        if (graphics) {
            sizef = graphics->MeasureString(label, font);
            width = sizef.get_Width() + 10;
            height = sizef.get_Height() + 10;
        }
    }
    return PR_SUCCESS;
}
