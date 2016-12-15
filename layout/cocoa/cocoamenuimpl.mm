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

#include "cocoawindowimpl.h"
#include "cocoamenuimpl.h"
#include "cocoamenubarimpl.h"

CocoaMenuImpl::CocoaMenuImpl() : m_menu(NULL)
{
}

CocoaMenuImpl::~CocoaMenuImpl()
{
    if (m_menu)
        [m_menu release];
}

PRStatus CocoaMenuImpl::Create()
{
    m_menu = [NSMenu alloc];
    if (m_menu) {

        // by default, we want to control enabling and disabling of the menu

        [m_menu setAutoenablesItems: NO];

        WidgetImpl *parentImpl = GetParent();

        if (parentImpl) {

            CocoaMenubarImpl *menubarImpl = dynamic_cast<CocoaMenubarImpl *>(parentImpl);
            if (menubarImpl) {
                return menubarImpl->AddToMenubar(m_menu);
            }
        }
    }
	return PR_FAILURE;
}

PRStatus CocoaMenuImpl::SetLabel(const string& label)
{
    if (m_menu) {
        NSString *nstitle = [NSString stringWithCString: label.c_str()];
        if (nstitle) {
            [m_menu setTitle: nstitle];
			return PR_SUCCESS;
		}
	}
	return PR_FAILURE;
}

PRStatus CocoaMenuImpl::SetShortcut(const string& shortcut)
{
    // dump it on the floor
	return PR_SUCCESS;
}

PRStatus 
CocoaMenuImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    x = y = width = height = 0;

    return PR_SUCCESS;
}

PRStatus 
CocoaMenuImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    return PR_SUCCESS;
}

PRStatus
CocoaMenuImpl::Show()
{
    return ShowImpl();
}

PRStatus
CocoaMenuImpl::Hide()
{
    return HideImpl();
}

PRStatus
CocoaMenuImpl::Enable()
{
    return PR_SUCCESS; 
}

PRStatus
CocoaMenuImpl::Disable()
{
    return PR_SUCCESS; 
}

PRStatus
CocoaMenuImpl::RemoveMenuItem(NSMenuItem *menuItem)
{
    if (!menuItem)
        return PR_FAILURE;
    [menuItem retain];
    [m_menu removeItem: menuItem];
    return PR_SUCCESS;
}
