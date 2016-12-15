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
#include "gtkmenuimpl.h"
#include "gtkmenubarimpl.h"

GtkMenuImpl::GtkMenuImpl() : m_menu(NULL), m_menuItem(NULL), 
    m_accelGroup(NULL)
{
}

GtkMenuImpl::~GtkMenuImpl()
{
}

PRStatus GtkMenuImpl::Create()
{
    m_menuItem = m_widget = gtk_menu_item_new(); 
    if (m_menuItem) {
        m_menu = gtk_menu_new();

        if (m_menu) {
            gtk_menu_item_set_submenu(GTK_MENU_ITEM(m_menuItem), m_menu);
        }

        WidgetImpl *parentImpl = GetParent();

        if (parentImpl) {

            GtkMenubarImpl *menubarImpl = dynamic_cast<GtkMenubarImpl *>(parentImpl);
            if (menubarImpl) {
                gtk_widget_show(GTK_WIDGET(m_menu));
                gtk_widget_show(GTK_WIDGET(m_menuItem));
                return menubarImpl->AddToMenubar(m_menuItem);
            }
        }
    }
	return PR_FAILURE;
}

PRStatus GtkMenuImpl::SetLabel(const string& label)
{
    if (m_menuItem) {
        GtkWidget *wlabel;

        wlabel = gtk_label_new(const_cast<char *>(label.c_str()));
        if (wlabel) {
            gtk_container_add(GTK_CONTAINER(m_menuItem), wlabel);
            gtk_widget_show(wlabel);
            return PR_SUCCESS;
        }
	}
	return PR_FAILURE;
}

PRStatus GtkMenuImpl::SetShortcut(const string& shortcut)
{
    GdkModifierType accel_mods;
    string accel;
    guint accel_key;

    accel = "<alt>" + shortcut;
    gtk_accelerator_parse(accel.c_str(), &accel_key, &accel_mods);

    if (!m_accelGroup) {
        m_accelGroup = gtk_accel_group_new();
        gtk_accel_group_ref(m_accelGroup);
    }

    gtk_widget_add_accelerator(m_menuItem, "activate", m_accelGroup,
        accel_key, accel_mods, GTK_ACCEL_VISIBLE);

    return PR_SUCCESS;
}

PRStatus
GtkMenuImpl::Show()
{
    if (m_menu)
        return ShowImpl(); 
    return PR_FAILURE;
}

PRStatus
GtkMenuImpl::Hide()
{
    if (m_menu)
        return HideImpl();
    return PR_FAILURE;
}

PRStatus
GtkMenuImpl::Enable()
{
    return PR_SUCCESS;
}

PRStatus
GtkMenuImpl::Disable()
{
    return PR_SUCCESS;
}

PRStatus
GtkMenuImpl::AddMenuItem(GtkWidget *menuItem)
{
    if (m_menu && menuItem) {
        gtk_menu_append(GTK_MENU(m_menu), menuItem);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
GtkMenuImpl::RemoveMenuItem(GtkWidget *menuItem)
{
    return PR_FAILURE;
}
