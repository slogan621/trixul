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

#include "gtkwindowimpl.h"
#include "gtkmenuitemimpl.h"
#include "gtkmenubarimpl.h"
#include "gtkmenuimpl.h"
#include "gtkappimpl.h"
#include "widgetimpl.h"

//#include "gtkmenuitemaction.h"

GtkMenuItemImpl::GtkMenuItemImpl() : m_menuitem(NULL), 
    m_menuItemClass(MenuItemClassNone), m_isOverride(false),
    m_accelGroup(NULL)
{
}

GtkMenuItemImpl::~GtkMenuItemImpl()
{
}

extern "C" {

gint HandleMenuItemCommandThunk(GtkWidget *widget, gpointer callbackData)
{
    GtkMenuItemImpl *pMenuItemImpl = (GtkMenuItemImpl *) callbackData;
    if (pMenuItemImpl)
        pMenuItemImpl->HandleCommand();
    return TRUE;
}

} // extern "C"

PRStatus GtkMenuItemImpl::Create()
{
    GtkMenubarImpl *menuBarImpl;

    // menu items must have menu parents in the DOM

    WidgetImpl *parentImpl = GetParent();

    if (!parentImpl) 
        return PR_FAILURE;

    GtkMenuImpl *menuImpl = dynamic_cast<GtkMenuImpl *>(parentImpl);
    if (!menuImpl)
        return PR_FAILURE;

    // likewise, menus must have menubar parents in the DOM
    
    parentImpl = menuImpl->GetParent();

    if (!parentImpl) 
        return PR_FAILURE;

    menuBarImpl = dynamic_cast<GtkMenubarImpl *>(parentImpl);
    if (!menuBarImpl)
        return PR_FAILURE;

    m_menuitem = m_widget = gtk_menu_item_new();

    if (m_menuitem && menuImpl) {
        gtk_widget_show(GTK_WIDGET(m_menuitem));
        gtk_signal_connect(GTK_OBJECT(m_menuitem), "activate",
            GTK_SIGNAL_FUNC(HandleMenuItemCommandThunk), this);

        return menuImpl->AddMenuItem(m_menuitem);
    } 

    return PR_FAILURE;
}

PRStatus GtkMenuItemImpl::Remove()
{
    WidgetImpl *parentImpl = GetParent();
    PRStatus ret;

    if (!parentImpl) 
        return PR_FAILURE;

    GtkMenuImpl *menuImpl = dynamic_cast<GtkMenuImpl *>(parentImpl);
    if (!menuImpl)
        return PR_FAILURE;
    ret = menuImpl->RemoveMenuItem(m_menuitem);
    return ret;
}

PRStatus GtkMenuItemImpl::HandleCommand()
{
    PRStatus ret = PR_SUCCESS;

    NotifyMenuItemClick();

    // handle special cases based on menuitem class

    MenuItemClass menuItemClass;
    menuItemClass = GetClass();

    switch(menuItemClass)
    {
    case MenuItemClassClose:
        {
            WidgetImpl *top = GetRootWidget();
        
            if (top) {
                GtkWindowImpl *winImpl = dynamic_cast<GtkWindowImpl *>(top);
                if (winImpl) {
                    winImpl->Close();
                }
            }
        }
        break;
    case MenuItemClassQuit:
        {
            GtkAppImpl *appImpl = GtkAppImpl::GetGtkAppImplInstance();
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

PRStatus GtkMenuItemImpl::SetLabel(const string& label)
{
    MenuItemClass menuItemClass = GetClass();

    string tmp(label); 
                        
    string::size_type pos = tmp.find("&",0);
    while (pos != string::npos) {
        tmp.replace(pos, 1, "");
        pos = tmp.find("&",0);
    }           
        
    m_label = tmp;

    if (menuItemClass == MenuItemClassSeparator)
        return PR_SUCCESS;

    if (m_menuitem) {
        GtkWidget *wlabel;

        wlabel = gtk_accel_label_new(const_cast<char *>(tmp.c_str()));
        if (wlabel) {
            gtk_misc_set_alignment(GTK_MISC(wlabel), 0.0, 0.5);
            gtk_container_add(GTK_CONTAINER(m_menuitem), wlabel);
            gtk_accel_label_set_accel_widget(GTK_ACCEL_LABEL(wlabel), 
                m_menuitem);
            gtk_widget_show(wlabel);
            return PR_SUCCESS;
        }
    }
	return PR_FAILURE;
}

PRStatus GtkMenuItemImpl::SetShortcut(const string& shortcut)
{
    string accel;
    guint accel_key;
    GdkModifierType accel_mods;

    accel = "<ctrl>" + shortcut;
    gtk_accelerator_parse(accel.c_str(), &accel_key, &accel_mods);

    if (!m_accelGroup) {
        m_accelGroup = gtk_accel_group_new();
        gtk_accel_group_ref(m_accelGroup);
    }

    gtk_widget_add_accelerator(m_menuitem, "activate", m_accelGroup,
        accel_key, accel_mods, GTK_ACCEL_VISIBLE);

	return PR_SUCCESS;
}

PRStatus
GtkMenuItemImpl::Show()
{
    return ShowImpl();
}

PRStatus
GtkMenuItemImpl::Hide()
{
    return HideImpl();
}

PRStatus
GtkMenuItemImpl::Enable()
{
    return EnableImpl();
}

PRStatus
GtkMenuItemImpl::Disable()
{
    return DisableImpl();
}

