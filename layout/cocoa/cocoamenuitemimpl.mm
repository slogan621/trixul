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
#include "cocoamenuitemimpl.h"
#include "cocoamenubarimpl.h"
#include "cocoamenuimpl.h"
#include "cocoaappimpl.h"
#include "widgetimpl.h"

CocoaMenuItemImpl::CocoaMenuItemImpl() : m_menuitem(NULL), 
    m_menuItemClass(MenuItemClassNone), m_isOverride(false),
    m_release(false), m_action(NULL)

{
}

CocoaMenuItemImpl::~CocoaMenuItemImpl()
{
    if (m_menuitem && m_release)
        [m_menuitem release];

    if (m_action)
        [m_action release];
}

PRStatus CocoaMenuItemImpl::Create()
{
    NSMenuItem *menuItem;
    CocoaMenubarImpl *menuBarImpl;

    MenuItemClass menuItemClass = GetClass();
    bool create = true;
    bool add = true;

    // menu items must have menu parents in the DOM

	WidgetImpl *parentImpl = GetParent();

	if (!parentImpl) 
        return PR_FAILURE;

    CocoaMenuImpl *menuImpl = dynamic_cast<CocoaMenuImpl *>(parentImpl);
    if (!menuImpl)
        return PR_FAILURE;

    // likewise, menus must have menubar parents in the DOM
    
	parentImpl = menuImpl->GetParent();

	if (!parentImpl) 
        return PR_FAILURE;

    menuBarImpl = dynamic_cast<CocoaMenubarImpl *>(parentImpl);
    if (!menuBarImpl)
        return PR_FAILURE;

    // filter for certain menu classes. for exampple, don't create a 
    // quit or exit menu item, since this is done in the Apple menu.
    // we can get the implementations by querying the menu bar widget.
    // with the implementations in hand, we can then cause their 
    // use to trigger an action back on this object.

    bool setAction = true;
    switch (menuItemClass)
    {
    case MenuItemClassQuit:
        menuItem = menuBarImpl->GetQuitMenuItem();
        m_isOverride = true;
        create = false;
        m_release = false;
        add = false;
        break;
    case MenuItemClassPreferences:
        menuItem = menuBarImpl->GetPreferencesMenuItem();
        m_isOverride = true;
        create = false;
        m_release = false;
        add = false;
        break;
    case MenuItemClassSeparator:

        // We have an interesting dilemna here. If the separator is followed 
        // by content that is not created (like the Quit menu item in a
        // File menu), and we hit another separator or the end of the menu 
        // before another valid menu item (other than a separator) is added, 
        // we will have a dangling (or redundant) separator in the menu. So,
        // we need to remember this menu item, and then check to see if it
        // ends up being either the last menu item in the menu, or if its
        // immediate successor is a separator. If either is the case, this
        // separator has to go. This should be easiest to do after the menu
        // has been created, we can walk the menu children for each child and 
        // look for adjacent separators, or a separator at the end and blow
        // whichever of the two cases we find away. See Menu::CreateChildren()

        menuItem = [NSMenuItem separatorItem]; 
        [menuItem retain];
        create = false;
        break;
    case MenuItemClassCut:
    case MenuItemClassCopy:
    case MenuItemClassPaste:
    case MenuItemClassClear:
    case MenuItemClassSelectAll:
        setAction = false;
        break;
    default:
        break;
    }

    if (setAction) {
	    m_action = [MenuItemAction alloc];
	    [m_action setHandler: this];
    }

    if (create) {
	    menuItem = m_menuitem = 
            [[NSMenuItem alloc] initWithTitle:@"" action:nil keyEquivalent:@""];
    }
    else
        m_menuitem = menuItem;

	if (menuItem) {
        if (setAction) {
		    [menuItem setTarget:m_action];
		    [menuItem setAction:@selector(onClick:)];
        } else {

            // let the system determine who handles these items.
            // see http://developer.apple.com/documentation/Cocoa/Conceptual/MenuList/Tasks/EnablingMenuItems.html#//apple_ref/doc/uid/20000261-74653
		    [menuItem setTarget:nil];

            switch (menuItemClass) {
                case MenuItemClassCut:
		            [menuItem setAction:@selector(cut:)];
                    break;
                case MenuItemClassCopy:
		            [menuItem setAction:@selector(copy:)];
                    break;
                case MenuItemClassPaste:
		            [menuItem setAction:@selector(paste:)];
                    break;
                case MenuItemClassClear:
		            [menuItem setAction:@selector(delete:)];
                    break;
                case MenuItemClassSelectAll:
		            [menuItem setAction:@selector(selectAll:)];
                    break;
                default:
                    printf("Unassigned selector in CocoaMenuItemImpl!\n");
                    break;
            }
        }
    }

    if (menuItem && add)
    {
        if (menuImpl) {
            NSMenu *parentMenu = menuImpl->GetMenu();

            // add the menuitem to the parent view

	        if (parentMenu) {
			    [parentMenu addItem: menuItem];
                if (!setAction) {
                    // once again, see http://developer.apple.com/documentation/Cocoa/Conceptual/MenuList/Tasks/EnablingMenuItems.html#//apple_ref/doc/uid/20000261-74653
                    [parentMenu setAutoenablesItems: YES];    
                }
			    return PR_SUCCESS;
            }
		}
	} else if (menuItem)
        return PR_SUCCESS;

	return PR_FAILURE;
}

PRStatus CocoaMenuItemImpl::Remove()
{
	WidgetImpl *parentImpl = GetParent();
    PRStatus ret;

	if (!parentImpl) 
        return PR_FAILURE;

    CocoaMenuImpl *menuImpl = dynamic_cast<CocoaMenuImpl *>(parentImpl);
    if (!menuImpl)
        return PR_FAILURE;
    ret = menuImpl->RemoveMenuItem(m_menuitem);
    [m_menuitem release];
    return ret;
}

PRStatus CocoaMenuItemImpl::HandleCommand()
{
    PRStatus ret = PR_SUCCESS;

    NotifyMenuItemClick();

    // handle special cases for MacOS X based on menuitem class

    MenuItemClass menuItemClass;
    menuItemClass = GetClass();

    NSWindow *nsWindow = NULL;

    WidgetImpl *top = GetRootWidget();
        
    if (top) {
        CocoaWindowImpl *winImpl = dynamic_cast<CocoaWindowImpl *>(top);
        if (winImpl) 
            nsWindow = winImpl->GetNSWindow();
    }

    switch(menuItemClass)
    {
    case MenuItemClassClose:
        {
            WidgetImpl *top = GetRootWidget();
        
            if (top) {
                CocoaWindowImpl *winImpl = dynamic_cast<CocoaWindowImpl *>(top);
                if (winImpl) {
                    winImpl->Close();
                }
            }
        }
        break;
    case MenuItemClassQuit:
        {
            CocoaAppImpl *appImpl = CocoaAppImpl::GetCocoaAppImplInstance();
            if (appImpl)
            {
                ret = appImpl->Shutdown();
            }
        }
        break;
    case MenuItemClassPageSetup:
        {
            NSView *nsView;
   
            // get the view of the containing window

            WidgetImpl *top = GetRootWidget();
        
            if (top) {
                CocoaWindowImpl *winImpl = dynamic_cast<CocoaWindowImpl *>(top);
                if (winImpl) {
                    nsView =  winImpl->GetView();
                    if (nsView)
                        [[NSPageLayout pageLayout] runModal];
                }
            }
        }
        break;
    case MenuItemClassPrint:
        {
            NSView *nsView;
   
            // get the view of the containing window

            WidgetImpl *top = GetRootWidget();
        
            if (top) {
                CocoaWindowImpl *winImpl = dynamic_cast<CocoaWindowImpl *>(top);
                if (winImpl) {
                    nsView =  winImpl->GetView();
                    if (nsView)
                        [[NSPrintOperation printOperationWithView:nsView] runOperation];
                }
            }
        }
        break;

/* 

From http://homepage.mac.com/svc/cocoa-objc-mac-os-x/

- (void)copy:(id)sender {
    NSString *contents = // from somewhere
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard declareTypes:[NSArray arrayWithObject:NSStringPboardType] owner:nil];
    [pasteboard setString:contents forType:NSStringPboardType];
}
   
- (void)paste:(id)sender {
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    NSString *type = [pasteboard availableTypeFromArray:[NSArray arrayWithObject:NSStringPboardType]];
    if (type != nil) {
        NSString *contents = [pasteboard stringForType:type];
        if (contents != nil) {
            // use somewhere
        }
    }
}

From Apple Docs

if ( [[[self window] firstResponder] isKindOfClass:[NSTextView class]] && 

   [window fieldEditor:NO forObject:nil] != nil ) { 

        NSTextField *field = [[[self window] firstResponder] delegate];

        if (field == self) {

            // do something based upon first-responder status

        }
}

*/

#if 0
    case MenuItemClassCut:
        {
            if (nsWindow)
            {
                NSView *firstResponder = [nsWindow firstResponder];
                if (firstResponder) {
                    if ([firstResponder isKindOfClass:[NSTextView class]]) {
                        NSTextField *field = [firstResponder delegate];
                        if (field) {
                            [field onCut];
                        }
                    }
                }
            }
        }
        break;
    case MenuItemClassCopy:
        {
            if (nsWindow)
            {
                NSView *firstResponder = [nsWindow firstResponder];
                if (firstResponder) {
                    if ([firstResponder isKindOfClass:[NSTextView class]]) {
                        NSTextField *field = [firstResponder delegate];
                        if (field) {
                            [field onCopy];
                        }
                    }
                }
            }
        }
        break;
    case MenuItemClassPaste:
        {
            if (nsWindow)
            {
                NSView *firstResponder = [nsWindow firstResponder];
                NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
                NSString *type = [pasteboard availableTypeFromArray:[NSArray arrayWithObject:NSStringPboardType]];
                if (type != nil && firstResponder) {
                    if ([firstResponder isKindOfClass:[NSTextView class]]) {
                        NSTextField *field = [firstResponder delegate];
                        if (field) {
                            [field onPaste];
                        }
                    }
                }
            }
        }
        break;
    case MenuItemClassClear:
        {
            if (nsWindow)
            {
                NSView *firstResponder = [nsWindow firstResponder];
                NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
                NSString *type = [pasteboard availableTypeFromArray:[NSArray arrayWithObject:NSStringPboardType]];
                if (type != nil && firstResponder) {
                    if ([firstResponder isKindOfClass:[NSTextView class]]) {
                        NSTextField *field = [firstResponder delegate];
                        if (field) {
                            [field onClear];
                        }
                    }
                }
            }
        }
        break;
    case MenuItemClassSelectAll:
        {
            if (nsWindow)
            {
                NSView *firstResponder = [nsWindow firstResponder];
                NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
                NSString *type = [pasteboard availableTypeFromArray:[NSArray arrayWithObject:NSStringPboardType]];
                if (type != nil && firstResponder) {
                    if ([firstResponder isKindOfClass:[NSTextView class]]) {
                        NSTextField *field = [firstResponder delegate];
                        if (field) {
                            [field onSelectAll];
                        }
                    }
                }
            }
        }
        break;
#endif
    default:
        break;
    }
    return ret;
}

PRStatus CocoaMenuItemImpl::SetLabel(const string& label)
{
    string tmp(label);

    string::size_type pos = tmp.find("&",0);
    while (pos != string::npos) {
        tmp.replace(pos, 1, "");
        pos = tmp.find("&",0);
    }

    if (m_menuitem) {
        NSString *nstitle = [NSString stringWithCString: tmp.c_str()];
        if (nstitle) {
            [m_menuitem setTitle: nstitle];
            return PR_SUCCESS;
        }
    }
	return PR_FAILURE;
}

PRStatus CocoaMenuItemImpl::SetShortcut(const string& shortcut)
{
    if (m_menuitem) {
        NSString *nstitle = [NSString stringWithCString: shortcut.c_str()];
        if (nstitle) {
            [m_menuitem setKeyEquivalent: nstitle];
            return PR_SUCCESS;
        }
    }
	return PR_FAILURE;
}

PRStatus 
CocoaMenuItemImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    x = y = width = height = 0;
    return PR_SUCCESS;
}

PRStatus 
CocoaMenuItemImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    return PR_SUCCESS;
}

PRStatus
CocoaMenuItemImpl::Show()
{
    return ShowImpl();
}

PRStatus
CocoaMenuItemImpl::Hide()
{
    return HideImpl();
}

PRStatus
CocoaMenuItemImpl::Enable()
{
    printf("In CocoaMenuItemImpl::Enable()\n");
    if (m_menuitem) {
        [m_menuitem setEnabled:YES];
        return PR_SUCCESS; 
    }
    return PR_FAILURE;
}

PRStatus
CocoaMenuItemImpl::Disable()
{
    printf("In CocoaMenuItemImpl::Disable()\n");
    if (m_menuitem) {
        [m_menuitem setEnabled:NO];
        return PR_SUCCESS; 
    }
    return PR_FAILURE;
}

