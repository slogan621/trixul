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
#include "windowsmenubarimpl.h"

WindowsMenubarImpl::WindowsMenubarImpl() : m_menubar(NULL)
{
}

WindowsMenubarImpl::~WindowsMenubarImpl()
{
}

PRStatus WindowsMenubarImpl::PreCreate()
{
    return PR_SUCCESS;
}

PRStatus WindowsMenubarImpl::PostCreate()
{
    return PR_SUCCESS;
}

PRStatus WindowsMenubarImpl::Create()
{
    m_menubar = __gc new MainMenu();
    if (m_menubar) {

        if (!m_formParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) 
                SetFormParent(top);
        }

        if (m_formParent) {
            m_formParent->Menu = m_menubar;
            return PR_SUCCESS;
        }
    }
	return PR_FAILURE;
}

PRStatus 
WindowsMenubarImpl::AddToMenubar(System::Windows::Forms::MenuItem *menu)
{
    if (!menu || !m_menubar)
        return PR_FAILURE;
    m_menubar->MenuItems->Add(menu);
    return PR_SUCCESS;
}

PRStatus 
WindowsMenubarImpl::AddToMenubar(System::Windows::Forms::MenuItem *menu, 
    int index)
{
    return AddToMenubar(menu);
}

PRStatus 
WindowsMenubarImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    x = y = width = height = 0;

    return PR_SUCCESS;
}

PRStatus 
WindowsMenubarImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    return PR_SUCCESS;
}

PRStatus
WindowsMenubarImpl::Show()
{
    return PR_SUCCESS;
}

PRStatus
WindowsMenubarImpl::Hide()
{
    return PR_SUCCESS;
}

PRStatus
WindowsMenubarImpl::Enable()
{
    return PR_SUCCESS;
}

PRStatus
WindowsMenubarImpl::Disable()
{
    return PR_SUCCESS;
}

