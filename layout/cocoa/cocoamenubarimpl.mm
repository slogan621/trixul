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
#include "cocoamenubarimpl.h"

CocoaMenubarImpl::CocoaMenubarImpl() : m_quitMenuItem(NULL),
    m_preferencesMenuItem(NULL)
{
	m_menubar = NULL;
}

CocoaMenubarImpl::~CocoaMenubarImpl()
{
    NSMenuItem *menuItem;

    menuItem = GetPreferencesMenuItem();
    if (menuItem)
        [menuItem release];

    menuItem = GetQuitMenuItem();
    if (menuItem)
        [menuItem release];
}

PRStatus CocoaMenubarImpl::PreCreate()
{
    return CreateAppleMenu();
}

PRStatus CocoaMenubarImpl::PostCreate()
{
    return CreateWindowMenu();
}

PRStatus CocoaMenubarImpl::Create()
{
    m_menubar = [[NSMenu alloc] initWithTitle: @""];
    if (m_menubar) {
        [NSApp setMainMenu: m_menubar];
        [m_menubar release];
        return PR_SUCCESS;
    }
	return PR_FAILURE;
}

PRStatus 
CocoaMenubarImpl::AddToMenubar(NSMenu *menu)
{
    NSMenuItem *dummyItem = [[NSMenuItem alloc] initWithTitle:@""
        action:nil keyEquivalent:@""];
    [dummyItem setSubmenu:menu];
    [m_menubar addItem:dummyItem];
    [dummyItem release];
    return PR_SUCCESS;
}

PRStatus 
CocoaMenubarImpl::AddToMenubar(NSMenu *menu, int index)
{
    NSMenuItem *dummyItem = [[NSMenuItem alloc] initWithTitle:@""
        action:nil keyEquivalent:@""];
    [dummyItem setSubmenu:menu];
    [m_menubar insertItem:dummyItem atIndex:index];
    [dummyItem release];
    return PR_SUCCESS;
}

PRStatus
CocoaMenubarImpl::CreateAppleMenu()
{
    NSMenuItem *menuitem;
    // Create the application (Apple) menu.
    NSMenu *menuApp = [[NSMenu alloc] initWithTitle: @"Apple Menu"];

    NSMenu *menuServices = [[NSMenu alloc] initWithTitle: @"Services"];
    [NSApp setServicesMenu:menuServices];

    menuitem = [[NSMenuItem alloc] initWithTitle:@"Preferences..." action:nil keyEquivalent:@","];
    [menuApp addItem: menuitem];
    SetPreferencesMenuItem(menuitem);
    [menuApp addItem: [NSMenuItem separatorItem]];
    menuitem = [[NSMenuItem alloc] initWithTitle: @"Services"
        action:nil keyEquivalent:@""];
    [menuitem setSubmenu:menuServices];
    [menuApp addItem: menuitem];
    [menuitem release];

    [menuApp addItem: [NSMenuItem separatorItem]];
    menuitem = [[NSMenuItem alloc] initWithTitle:@"Hide"
        action:@selector(hide:) keyEquivalent:@""];
    [menuitem setTarget: NSApp];
    [menuApp addItem: menuitem];
    [menuitem release];
    menuitem = [[NSMenuItem alloc] initWithTitle:@"Hide Others"
        action:@selector(hideOtherApplications:) keyEquivalent:@""];
    [menuitem setTarget: NSApp];
    [menuApp addItem: menuitem];
    [menuitem release];
    menuitem = [[NSMenuItem alloc] initWithTitle:@"Show All"
        action:@selector(unhideAllApplications:) keyEquivalent:@""];
    [menuitem setTarget: NSApp];
    [menuApp addItem: menuitem];
    [menuitem release];
    [menuApp addItem: [NSMenuItem separatorItem]];
    menuitem = [[NSMenuItem alloc] initWithTitle:@"Quit"
        action:@selector(terminate:) keyEquivalent:@"q"];
    [menuitem setTarget: NSApp];
    [menuApp addItem: menuitem];
    SetQuitMenuItem(menuitem);
    [menuitem release];

    [NSApp setAppleMenu:menuApp];
    AddToMenubar(menuApp);
    [menuApp release];
    return PR_SUCCESS;
}

PRStatus 
CocoaMenubarImpl::CreateWindowMenu()
{       
    NSMenu *menuWindows = [[NSMenu alloc] initWithTitle: @"Window"];
    
    [menuWindows addItemWithTitle:@"Minimize"
        action:@selector(performMiniaturize:) keyEquivalent:@""];
    [menuWindows addItem: [NSMenuItem separatorItem]];
    [menuWindows addItemWithTitle:@"Bring All to Front"
        action:@selector(arrangeInFront:) keyEquivalent:@""];

    [NSApp setWindowsMenu:menuWindows];
    AddToMenubar(menuWindows);
    [menuWindows release];
    return PR_SUCCESS;
}   

PRStatus 
CocoaMenubarImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    x = y = width = height = 0;

    return PR_SUCCESS;
}

PRStatus 
CocoaMenubarImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    return PR_SUCCESS;
}

PRStatus
CocoaMenubarImpl::Show()
{
    return PR_SUCCESS;
}

PRStatus
CocoaMenubarImpl::Hide()
{
    return PR_SUCCESS;
}

PRStatus
CocoaMenubarImpl::Enable()
{
    return PR_SUCCESS;
}

PRStatus
CocoaMenubarImpl::Disable()
{
    return PR_SUCCESS;
}

