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

#include "windowswindowimpl.h"
#include "formimpl.h"

#include "../radiogroupmanager.h"

WindowResizeHelper::WindowResizeHelper(WindowsWindowImpl *impl) 
{
    m_impl = impl;
}

void WindowResizeHelper::OnResizeHandler(Object *sender, System::EventArgs* e)
{
    if (m_impl && sender) {
        System::Windows::Forms::Control *control = dynamic_cast<System::Windows::Forms::Control *>(sender);
        if (control) {
            Rectangle r = control->ClientRectangle;
            int x, y, width, height;

            x = r.Left; 
            y = r.Top; 
            width = r.Right - r.Left; 
            height = (r.Bottom - r.Top); 
            m_impl->HandleResize(x, y, width, height);
        } 
    }
}

WindowGotFocusHelper::WindowGotFocusHelper(WindowsWindowImpl *impl) 
{
    m_impl = impl;
}

void WindowGotFocusHelper::OnGotFocusHandler(Object *sender, System::EventArgs* e)
{
    if (m_impl && sender) {
        m_impl->HandleRaise();
    }
}

WindowsWindowImpl::WindowsWindowImpl() : m_form(NULL)
{
}

WindowsWindowImpl::~WindowsWindowImpl()
{
    RadioGroupManager::RemoveManager(GetDocument());
}

PRStatus WindowsWindowImpl::Create()
{
    RadioGroupManager::AddManager(GetDocument());

    m_form = __gc new FormImpl();
    if (m_form) {
        m_resizeHelper = __gc new WindowResizeHelper(this);
        m_gotFocusHelper = __gc new WindowGotFocusHelper(this);
        SetForm(m_form);
        m_form->add_Resize(new EventHandler(static_cast<WindowResizeHelper *>(m_resizeHelper),&WindowResizeHelper::OnResizeHandler));
        m_form->add_Activated(new EventHandler(static_cast<WindowGotFocusHelper *>(m_gotFocusHelper),&WindowGotFocusHelper::OnGotFocusHandler));
	    return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus WindowsWindowImpl::Enable()
{
    return PR_SUCCESS;
}

PRStatus WindowsWindowImpl::Disable()
{
    return PR_SUCCESS;
}

PRStatus WindowsWindowImpl::Show()
{
    if (m_form) {
        m_form->Show();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus WindowsWindowImpl::Hide()
{
    if (m_form) {
        m_form->Hide();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus WindowsWindowImpl::Close()
{
    if (m_form) {
        m_form->Close();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus WindowsWindowImpl::HandleRaise()
{
    NotifyWindowRaise();
    return PR_SUCCESS;
}

PRStatus WindowsWindowImpl::HandleResize(const int &x, const int &y, 
    const int &width, const int &height)
{
    NotifyWindowResize(x, y, width, height);
    return PR_SUCCESS;
}

PRStatus WindowsWindowImpl::SetTitle(const string& title)
{
    if (m_form)
        m_form->Text = title.c_str();
    return PR_SUCCESS;
}

PRStatus WindowsWindowImpl::SetGeometry(const int &x, const int &y,
        const int &width, const int &height, const char &mask)
{
    if (m_form) {
        Size size;
        Point point;


        size = m_form->ClientSize;
        point = m_form->Location;

        if (mask & GEOM_X)
            point.X = x;
        if (mask & GEOM_Y)
            point.Y = y;
        if (mask & GEOM_WIDTH)
            size.Width = width;
        if (mask & GEOM_HEIGHT)
            size.Height = height;

        m_form->ClientSize=size;
        m_form->Location=point;
    } 

    return PR_SUCCESS;
}

PRStatus WindowsWindowImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    Size size, request(0, 0);
    
    if (m_form) {
        size = m_form->ClientSize;
        width = size.get_Width();
        height = size.get_Height();
    }
    return PR_SUCCESS;
}

