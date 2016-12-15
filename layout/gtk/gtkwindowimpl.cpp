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
#include "gtkresizeaction.h"

#include "../radiogroupmanager.h"

GtkWindowImpl::GtkWindowImpl() : m_fixed(NULL)
{
}

GtkWindowImpl::~GtkWindowImpl()
{
    RadioGroupManager::RemoveManager(GetDocument());
}

extern "C" {

gint HandleResizeThunk(GtkWidget *widget, GdkEvent *event, 
    gpointer callbackData)
{
    GtkWindowImpl *pWindowImpl = (GtkWindowImpl *) callbackData;
    return pWindowImpl->HandleResize(widget, event, NULL);
}

gint HandleRaiseThunk(GtkWidget *widget, GdkEvent *event, 
    gpointer callbackData)
{
    GtkWindowImpl *pWindowImpl = (GtkWindowImpl *) callbackData;
    return pWindowImpl->HandleRaise(widget, event, NULL);
}

} // extern "C"

gint GtkWindowImpl::HandleRaise(GtkWidget *widget, GdkEvent *event,
    gpointer callbackData)
{
    // notify the document so it can recompute layout

    if (event != (GdkEvent *) NULL && event->type == GDK_FOCUS_CHANGE) {
        GdkEventFocus *focus = (GdkEventFocus *) event;
        if (focus->in == TRUE)
            NotifyWindowRaise();
        return TRUE;
    }
    return FALSE;
}

gint GtkWindowImpl::HandleResize(GtkWidget *widget, GdkEvent *event,
    gpointer callbackData)
{
    // notify the document so it can recompute layout

    if (event != (GdkEvent *) NULL && event->type == GDK_CONFIGURE) {
        GdkEventConfigure *config = (GdkEventConfigure *) event;
        NotifyWindowResize(config->x, config->y, config->width, config->height);
        return TRUE;
    }
    return FALSE;
}

PRStatus GtkWindowImpl::Create()
{
    m_widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    if (m_widget) {

        gtk_window_set_policy(GTK_WINDOW(m_widget), 1, 1, 0);
        RadioGroupManager::AddManager(GetDocument());

        // instantiate a fixed widget that will contain all of the children
        // that are managed by this window

        m_fixed = gtk_fixed_new();
        if (m_fixed) {
            gtk_container_add(GTK_CONTAINER(m_widget), m_fixed);
            // register for resize events
            gtk_widget_set_events(m_widget, 
                GDK_STRUCTURE_MASK|GDK_FOCUS_CHANGE_MASK);
            gtk_signal_connect(GTK_OBJECT(m_widget), "configure_event", 
                GTK_SIGNAL_FUNC(HandleResizeThunk), this);
            gtk_signal_connect(GTK_OBJECT(m_widget), "focus_in_event", 
                GTK_SIGNAL_FUNC(HandleRaiseThunk), this);
            return PR_SUCCESS;
        }
    }
	return PR_FAILURE;
}

GtkWidget *GtkWindowImpl::GetImpl()
{
    return m_fixed;
}

PRStatus GtkWindowImpl::Show()
{
    if (m_widget && m_fixed) {
        gtk_widget_show(m_fixed); 
        return ShowImpl();
    }
	return PR_FAILURE;
}

PRStatus GtkWindowImpl::Hide()
{
    if (m_widget && m_fixed) {
        gtk_widget_hide(m_fixed); 
        return HideImpl();
    }
    return PR_FAILURE;
}

PRStatus GtkWindowImpl::Enable()
{
    return PR_SUCCESS;
}

PRStatus GtkWindowImpl::Disable()
{
    return PR_SUCCESS;
}

PRStatus GtkWindowImpl::Close()
{
    if (m_widget && m_fixed) {
        gtk_widget_destroy(m_fixed); 
        gtk_widget_destroy(m_widget); 
        return PR_SUCCESS; 
    }
    return PR_FAILURE;
}

PRStatus GtkWindowImpl::SetTitle(const string& title)
{
    if (m_widget) {
        gtk_window_set_title(GTK_WINDOW(m_widget), title.c_str());
        return PR_SUCCESS;
    }
	return PR_FAILURE;
}

