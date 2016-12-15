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

sdfsdfsdsdf
// Button Clicks

#include "buttoncallbackhelper.h"
#include "windowsbuttonimpl.h"
#include "commandhandler.h"

//#pragma managed 

ButtonCallbackHelper::ButtonCallbackHelper(CommandHandler *impl) 
{
    m_impl = impl;
}

void ButtonCallbackHelper::OnButtonClick(Object *sender, System::EventArgs* e)
{
    if (m_impl) 
        m_impl->HandleCommand();
}

// Window resize

#include "windowresizehelper.h"
#include "windowswindowimpl.h"

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

#include "windowgotfocushelper.h"

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

#include "controlgotfocushelper.h"

ControlGotFocusHelper::ControlGotFocusHelper(WindowsWidgetImpl *impl) 
{
printf("In control got focus ctor!!!\n");
    m_impl = impl;
}

void ControlGotFocusHelper::OnGotFocusHandler(Object *sender, System::EventArgs* e)
{
printf("In control got focus handler!!!\n");
    if (m_impl && sender) {
    }
}
// menuitem click

#include "menuitemcallbackhelper.h"
#include "windowsmenuitemimpl.h"

MenuItemCallbackHelper::MenuItemCallbackHelper(WindowsMenuItemImpl *impl) 
{
    m_impl = impl;
}

void MenuItemCallbackHelper::OnMenuItemClick(Object *sender, System::EventArgs* e)
{
    if (m_impl) 
        m_impl->HandleCommand();
}

