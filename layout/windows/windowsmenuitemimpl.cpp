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
#include "windowsmenuitemimpl.h"
#include "windowsmenubarimpl.h"
#include "windowsmenuimpl.h"
#include "windowsappimpl.h"
#include "widgetimpl.h"
#include "menuitemcallbackhelper.h"

#include "../clipboardhelper.h"


MenuItemCallbackHelper::MenuItemCallbackHelper(WindowsMenuItemImpl *impl) 
{
    m_impl = impl;
}

void MenuItemCallbackHelper::OnMenuItemClick(Object *sender, System::EventArgs* e)
{
    if (m_impl) 
        m_impl->HandleCommand();
}

WindowsMenuItemImpl::WindowsMenuItemImpl() : m_menuItem(NULL), 
    m_menuItemClass(MenuItemClassNone), m_isOverride(false)
{
}

WindowsMenuItemImpl::~WindowsMenuItemImpl()
{
}

PRStatus WindowsMenuItemImpl::Create()
{
    WindowsMenubarImpl *menuBarImpl;

    MenuItemClass menuItemClass = GetClass();
    bool create = true;
    bool add = true;

    // menu items must have menu parents in the DOM

    WidgetImpl *parentImpl = GetParent();

    if (!parentImpl) 
        return PR_FAILURE;

    WindowsMenuImpl *menuImpl = dynamic_cast<WindowsMenuImpl *>(parentImpl);
    if (!menuImpl)
        return PR_FAILURE;

    // likewise, menus must have menubar parents in the DOM
    
    parentImpl = menuImpl->GetParent();

    if (!parentImpl) 
        return PR_FAILURE;

    menuBarImpl = dynamic_cast<WindowsMenubarImpl *>(parentImpl);
    if (!menuBarImpl)
        return PR_FAILURE;

    if (menuItemClass == MenuItemClassSeparator) {
        m_menuItem = __gc new System::Windows::Forms::MenuItem("-");
    }
    else
        m_menuItem = __gc new System::Windows::Forms::MenuItem();
            
	if (m_menuItem) {
        if (!m_formParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetFormParent(top);
            }
        }

        if (m_formParent) {
            MenuItemCallbackHelper *pHelper;
            pHelper = m_menuItemHelper = __gc new MenuItemCallbackHelper(this);
            m_menuItem->add_Click(new
                EventHandler(pHelper,&MenuItemCallbackHelper::OnMenuItemClick));
        }

    }

    if (m_menuItem && menuImpl)
    {
        menuImpl->AddMenuItem(m_menuItem);
        return PR_SUCCESS;
    }

    return PR_FAILURE;
}

PRStatus WindowsMenuItemImpl::Remove()
{
    WidgetImpl *parentImpl = GetParent();
    PRStatus ret;

    if (!parentImpl) 
        return PR_FAILURE;

    WindowsMenuImpl *menuImpl = dynamic_cast<WindowsMenuImpl *>(parentImpl);
    if (!menuImpl)
        return PR_FAILURE;
    ret = menuImpl->RemoveMenuItem(m_menuItem);
    return ret;
}

PRStatus WindowsMenuItemImpl::HandleCommand()
{
    PRStatus ret = PR_SUCCESS;

    NotifyMenuItemClick();

    // handle special cases for MacOS X based on menuitem class

    MenuItemClass menuItemClass;
    menuItemClass = GetClass();

    WidgetImpl *top = GetRootWidget();
        
    switch(menuItemClass)
    {
    case MenuItemClassClose:
        {
            WidgetImpl *top = GetRootWidget();
        
            if (top) {
                WindowsWindowImpl *winImpl = dynamic_cast<WindowsWindowImpl *>(top);
                if (winImpl) {
                    winImpl->Close();
                }
            }
        }
        break;
    case MenuItemClassQuit:
        {
            WindowsAppImpl *appImpl = WindowsAppImpl::GetWindowsAppImplInstance();
            if (appImpl)
            {
                ret = appImpl->Shutdown();
            }
        }
        break;
    case MenuItemClassCut:
        {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                Document *doc = top->GetDocument();
                if (doc) {
                   doc->HandleClipboard(ClipboardHelper::ClipboardOpCut); 
                }
            }
        }
        break;
    case MenuItemClassCopy:
        {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                Document *doc = top->GetDocument();
                if (doc) {
                   doc->HandleClipboard(ClipboardHelper::ClipboardOpCopy); 
                }
            }
        }
        break;
    case MenuItemClassPaste:
        {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                Document *doc = top->GetDocument();
                if (doc) {
                   doc->HandleClipboard(ClipboardHelper::ClipboardOpPaste); 
                }
            }
        }
        break;
    case MenuItemClassClear:
        {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                Document *doc = top->GetDocument();
                if (doc) {
                   doc->HandleClipboard(ClipboardHelper::ClipboardOpClear); 
                }
            }
        }
        break;
    case MenuItemClassSelectAll:
        {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                Document *doc = top->GetDocument();
                if (doc) {
                   doc->HandleClipboard(ClipboardHelper::ClipboardOpSelectAll); 
                }
            }
        }
        break;
    default:
        break;
    }
    return ret;
}

PRStatus WindowsMenuItemImpl::SetLabel(const string& label)
{
    MenuItemClass menuItemClass;
    menuItemClass = GetClass();
    if (m_menuItem && menuItemClass != MenuItemClassSeparator) {
        m_menuItem->Text = label.c_str();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus WindowsMenuItemImpl::SetShortcut(const string& shortcut)
{
    MenuItemClass menuItemClass;
    menuItemClass = GetClass();
    System::Windows::Forms::Shortcut value;
    value = ConvertShortcut(shortcut.c_str());
    if (m_menuItem && menuItemClass != MenuItemClassSeparator) {
        m_menuItem->Shortcut = value; 
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

System::Windows::Forms::Shortcut
WindowsMenuItemImpl::ConvertShortcut(const char *shortcut)
{
    System::Windows::Forms::Shortcut ret;
    if (!shortcut || !strlen(shortcut))
        ret = System::Windows::Forms::Shortcut::None;
    else if (isupper(shortcut[0])) {
        switch(shortcut[0]) {
        case 'A':
            ret = System::Windows::Forms::CtrlShiftA;
            break;
        case 'B':
            ret = System::Windows::Forms::CtrlShiftB;
            break;
        case 'C':
            ret = System::Windows::Forms::CtrlShiftC;
            break;
        case 'D':
            ret = System::Windows::Forms::CtrlShiftD;
            break;
        case 'E':
            ret = System::Windows::Forms::CtrlShiftE;
            break;
        case 'F':
            ret = System::Windows::Forms::CtrlShiftF;
            break;
        case 'G':
            ret = System::Windows::Forms::CtrlShiftG;
            break;
        case 'H':
            ret = System::Windows::Forms::CtrlShiftH;
            break;
        case 'I':
            ret = System::Windows::Forms::CtrlShiftI;
            break;
        case 'J':
            ret = System::Windows::Forms::CtrlShiftJ;
            break;
        case 'K':
            ret = System::Windows::Forms::CtrlShiftK;
            break;
        case 'L':
            ret = System::Windows::Forms::CtrlShiftL;
            break;
        case 'M':
            ret = System::Windows::Forms::CtrlShiftM;
            break;
        case 'N':
            ret = System::Windows::Forms::CtrlShiftN;
            break;
        case 'O':
            ret = System::Windows::Forms::CtrlShiftO;
            break;
        case 'P':
            ret = System::Windows::Forms::CtrlShiftP;
            break;
        case 'Q':
            ret = System::Windows::Forms::CtrlShiftQ;
            break;
        case 'R':
            ret = System::Windows::Forms::CtrlShiftR;
            break;
        case 'S':
            ret = System::Windows::Forms::CtrlShiftS;
            break;
        case 'T':
            ret = System::Windows::Forms::CtrlShiftT;
            break;
        case 'U':
            ret = System::Windows::Forms::CtrlShiftU;
            break;
        case 'V':
            ret = System::Windows::Forms::CtrlShiftV;
            break;
        case 'W':
            ret = System::Windows::Forms::CtrlShiftW;
            break;
        case 'X':
            ret = System::Windows::Forms::CtrlShiftX;
            break;
        case 'Y':
            ret = System::Windows::Forms::CtrlShiftY;
            break;
        case 'Z':
            ret = System::Windows::Forms::CtrlShiftZ;
            break;
        default:
            ret = System::Windows::Forms::Shortcut::None;
            break;
        }
    } else {
        switch(toupper(shortcut[0])) {
        case 'A':
            ret = System::Windows::Forms::CtrlA;
            break;
        case 'B':
            ret = System::Windows::Forms::CtrlB;
            break;
        case 'C':
            ret = System::Windows::Forms::CtrlC;
            break;
        case 'D':
            ret = System::Windows::Forms::CtrlD;
            break;
        case 'E':
            ret = System::Windows::Forms::CtrlE;
            break;
        case 'F':
            ret = System::Windows::Forms::CtrlF;
            break;
        case 'G':
            ret = System::Windows::Forms::CtrlG;
            break;
        case 'H':
            ret = System::Windows::Forms::CtrlH;
            break;
        case 'I':
            ret = System::Windows::Forms::CtrlI;
            break;
        case 'J':
            ret = System::Windows::Forms::CtrlJ;
            break;
        case 'K':
            ret = System::Windows::Forms::CtrlK;
            break;
        case 'L':
            ret = System::Windows::Forms::CtrlL;
            break;
        case 'M':
            ret = System::Windows::Forms::CtrlM;
            break;
        case 'N':
            ret = System::Windows::Forms::CtrlN;
            break;
        case 'O':
            ret = System::Windows::Forms::CtrlO;
            break;
        case 'P':
            ret = System::Windows::Forms::CtrlP;
            break;
        case 'Q':
            ret = System::Windows::Forms::CtrlQ;
            break;
        case 'R':
            ret = System::Windows::Forms::CtrlR;
            break;
        case 'S':
            ret = System::Windows::Forms::CtrlS;
            break;
        case 'T':
            ret = System::Windows::Forms::CtrlT;
            break;
        case 'U':
            ret = System::Windows::Forms::CtrlU;
            break;
        case 'V':
            ret = System::Windows::Forms::CtrlV;
            break;
        case 'W':
            ret = System::Windows::Forms::CtrlW;
            break;
        case 'X':
            ret = System::Windows::Forms::CtrlX;
            break;
        case 'Y':
            ret = System::Windows::Forms::CtrlY;
            break;
        case 'Z':
            ret = System::Windows::Forms::CtrlZ;
            break;
        default:
            ret = System::Windows::Forms::Shortcut::None;
            break;
        }
    }
    return ret;
}

PRStatus 
WindowsMenuItemImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    x = y = width = height = 0;
    return PR_SUCCESS;
}

PRStatus 
WindowsMenuItemImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    return PR_SUCCESS;
}

PRStatus
WindowsMenuItemImpl::Enable()
{
    if (m_menuItem) {
        m_menuItem->Enabled = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE; 
}

PRStatus
WindowsMenuItemImpl::Disable()
{
    if (m_menuItem) {
        m_menuItem->Enabled = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE; 
}

PRStatus
WindowsMenuItemImpl::Show()
{
    return ShowImpl();
}

PRStatus
WindowsMenuItemImpl::Hide()
{
    return HideImpl();
}

