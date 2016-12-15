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

#include "menu.h"
#include "menuitem.h"

Menu::Menu() 
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_menu = factory->MakeMenu();
}

Menu::Menu(string& label)
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_menu = factory->MakeMenu();
	SetLabel(label);
}

Menu::~Menu()
{
    if (m_menu)
        delete m_menu;
}

PRStatus Menu::CreateChildren()
{
    // iterate the window's children, calling their create functions

    list <Element *>::iterator itr, itr2;
    WidgetImpl *thisImpl = GetImpl();

    if (!thisImpl) {  // box, grid don't have concrete impls
        Widget *w = dynamic_cast<Widget *>(this);
        WidgetImpl *impl;
        while ((w = dynamic_cast<Widget *>(w->GetParent())) != NULL) {
            impl = w->GetImpl();
            if (impl) {
                thisImpl = impl;
                break;
            }
        }
    }

    if (!thisImpl) {
        return PR_FAILURE;
    }

    list<Element *> children;
    children = GetChildren();

    for (itr = children.begin(); itr != children.end(); ++itr) {
        WidgetImpl *impl;

        Widget *w;

        w = dynamic_cast <Widget *>(*itr);
        if (w) {
            impl = w->GetImpl();
            if (impl) {
                impl->SetParent(thisImpl);
            }
            w->Create();
        }
    }

    // Make sure that we don't have any separators that are adjacent, or that 
    // are at the end of the bottom or top of the menu. If so, blow them away. 
    // This may happen most likely on MacOS X, but could happen anywhere.
    // See comments in CocoaMenuItemImpl::Create() for possible scenarios. 

    bool first = true, last = false;
    for (itr = children.begin(); itr != children.end(); ++itr) {
        if (itr != children.begin())
            first = false;
        if (itr == children.end())
            last = true;

        WidgetImpl *impl;
        MenuItem *menuItem;
        Widget *w;

        w = dynamic_cast <Widget *>(*itr);
        if (w) {
            menuItem = dynamic_cast<MenuItem *>(w);
            impl = w->GetImpl();
            if (impl && menuItem) {
                MenuItemImpl *menuItemImpl = dynamic_cast<MenuItemImpl *>(impl);
                if (menuItemImpl)
                {
                    bool isSeparator = menuItem->IsSeparator();

                    // simple case, separator at top or bottom of menu
                    if (isSeparator && (first || last)) {
                        menuItemImpl->Remove();
                    }
                    else if (isSeparator)
                    {
                        // make sure we find another visible, non-separator
                        // menu item before the end of the menu. otherwise, 
                        // blow the separator away

                        bool foundNonSeparatorNonOverride = false;

                        itr++;
                        for (itr2 = itr; itr2 != children.end(); ++itr2) {
                            WidgetImpl *impl;
                            Widget *w;
                            MenuItem *menuItem;

                            w = dynamic_cast <Widget *>(*itr2);
                            if (w) {
                                menuItem = dynamic_cast<MenuItem *>(w);
                                impl = w->GetImpl();
                                if (impl && menuItem) {
                                    MenuItemImpl *menuItemImpl = 
                                        dynamic_cast<MenuItemImpl *>(impl);
                                    if (menuItemImpl) {
                                        // an override menu item was replaced
                                        // by some other menu, e.g., a quit
                                        // menu item would be overridden by
                                        // the apple menu quit item, and would
                                        // not be created.
 
                                        bool isSeparator = menuItem->IsSeparator();
                                        bool isOverride = menuItemImpl->IsOverride();
                                        if (!isSeparator && !isOverride) {
                                            foundNonSeparatorNonOverride = true;
                                            break;      
                                        }
                                    }
                                }
                            }
                        }
                        itr--;
                        if (!foundNonSeparatorNonOverride)
                        {
                            // the separator is dangling, so blow it away
                            menuItemImpl->Remove();
                        }
                    }
                }
            }
        }
    }
    return PR_SUCCESS;
}

PRStatus Menu::SetLabel(string& label)
{
	AddAttribute(string("label"), label);

	if (m_menu)
		m_menu->SetLabel(label);
	return PR_SUCCESS;
}

string& Menu::GetLabel()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("label")); 
	if ( attribute ) 
		return attribute->GetValue();
	return failret;
}

PRStatus Menu::SetShortcut(string& shortcut)
{
	AddAttribute(string("shortcut"), shortcut);

	if (m_menu)
		m_menu->SetShortcut(shortcut);
	return PR_SUCCESS;
}

string& Menu::GetShortcut()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("shortcut")); 
	if ( attribute ) 
		return attribute->GetValue();
	return failret;
}

PRStatus Menu::Draw()
{
	return PR_SUCCESS;
}

PRStatus Menu::Show()
{
    if (m_menu)
	    m_menu->Show();
    return PR_SUCCESS;
}

PRStatus Menu::Hide()
{
    if (m_menu)
	    m_menu->Show();
    return PR_SUCCESS;
}

PRStatus Menu::Create()
{    
    PRStatus status;

    status = m_menu->Create();
    if (status == PR_SUCCESS) {
        status = SetLabel(GetLabel());
        status = SetShortcut(GetShortcut());
        status = CreateChildren();
    }
    return status;
}

PRStatus Menu::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_menu)
        m_menu->GetGeometry(x, y, width, height); 
    return PR_SUCCESS;
}

PRStatus Menu::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char& mask)
{
    if (m_menu)
        m_menu->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus Menu::ComputeLayout(const int &x, const int &y, const int &width,
    const int &height)
{
    return PR_SUCCESS;
}

PRStatus Menu::GetGeometryRequestImpl(int &x, int &y, int &width, int &height)
{
    if (m_menu)
        m_menu->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}
