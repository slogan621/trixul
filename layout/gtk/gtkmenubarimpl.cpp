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
#include "gtkmenubarimpl.h"

GtkMenubarImpl::GtkMenubarImpl() : m_menubar(NULL), 
    m_preferencesMenuItem(NULL), m_quitMenuItem(NULL)
{
	m_menubar = NULL;
}

GtkMenubarImpl::~GtkMenubarImpl()
{
    //GtkWidget *menuItem;

#if 0
    menuItem = GetPreferencesMenuItem();
    if (menuItem)
        [menuItem release];

    menuItem = GetQuitMenuItem();
    if (menuItem)
        [menuItem release];
#endif
}

PRStatus GtkMenubarImpl::PreCreate()
{
    return PR_SUCCESS; 
}

PRStatus GtkMenubarImpl::PostCreate()
{
    return PR_SUCCESS; 
}

PRStatus GtkMenubarImpl::Create()
{
    m_menubar = m_widget = gtk_menu_bar_new(); 
    if (m_menubar) {
        if (!m_fixedParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetFixedParent(top);
            }
        }

        if (m_fixedParent) {
            gtk_fixed_put(GTK_FIXED(m_fixedParent), m_menubar, 0, 0);
            Show();
#if 0
            gtk_signal_connect(GTK_OBJECT(m_widget), "clicked",
                GTK_SIGNAL_FUNC(HandleCommandThunk), this);
#endif
            return PR_SUCCESS;
        }

        return PR_SUCCESS;
    }
	return PR_FAILURE;
}

PRStatus 
GtkMenubarImpl::AddToMenubar(GtkWidget *menu)
{
    gtk_menu_bar_append(GTK_MENU_BAR(m_menubar), menu);
    return PR_SUCCESS;
}

PRStatus 
GtkMenubarImpl::AddToMenubar(GtkWidget *menu, int index)
{
    gtk_menu_bar_insert(GTK_MENU_BAR(m_menubar), menu, index);
    return PR_SUCCESS;
}

PRStatus
GtkMenubarImpl::Show()
{
    if (m_menubar)
        return ShowImpl();
    return PR_FAILURE;
}

PRStatus
GtkMenubarImpl::Hide()
{
    if (m_menubar)
        return HideImpl();
    return PR_FAILURE;
}

PRStatus
GtkMenubarImpl::Enable()
{
    return PR_SUCCESS;
}

PRStatus
GtkMenubarImpl::Disable()
{
    return PR_SUCCESS;
}


