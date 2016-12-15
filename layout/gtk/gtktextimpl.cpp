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
#include "gtktextimpl.h"

GtkTextImpl::GtkTextImpl()
{
}

GtkTextImpl::~GtkTextImpl()
{
	// destroy the text
}

PRStatus GtkTextImpl::Create()
{
    m_widget = gtk_entry_new();
    if (m_widget) {
        gtk_entry_set_editable(GTK_ENTRY(m_widget), true);
        if (!m_fixedParent) { 
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetFixedParent(top);
            }
        }

        RegisterFocusCallback();

        if (m_fixedParent) {
            gtk_fixed_put(GTK_FIXED(m_fixedParent), m_widget, 0, 0);
            return PR_SUCCESS;
        } 
    }
	return PR_FAILURE;
}

PRStatus GtkTextImpl::SetString(const string& value)
{
    if (m_widget) {
        gtk_entry_set_text(GTK_ENTRY(m_widget), value.c_str());
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus GtkTextImpl::SetIsEditable(const bool isEditable)
{
    if (m_widget) {
        gtk_entry_set_editable(GTK_ENTRY(m_widget), isEditable);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus GtkTextImpl::SetIsSelectable(const bool isSelectable)
{
    // XXX not controllable in Gtk+

    return PR_SUCCESS;
}

PRStatus    
GtkTextImpl::Show()
{   
    return ShowImpl();
}

PRStatus
GtkTextImpl::Hide()
{
    return HideImpl();
}       

PRStatus
GtkTextImpl::Enable()
{   
    return EnableImpl();
}

PRStatus
GtkTextImpl::Disable()
{
    return DisableImpl();
}       

PRStatus
GtkTextImpl::GetValue(string &v)
{
    if (m_widget) {
        v = gtk_entry_get_text(GTK_ENTRY(m_widget));
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}
    
PRStatus
GtkTextImpl::SetValue(const string &v)
{
    if (m_widget) {
        gtk_entry_set_text(GTK_ENTRY(m_widget), v.c_str());
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}  

void
GtkTextImpl::OnCut()
{
    if (m_widget) {
        gtk_editable_cut_clipboard(GTK_EDITABLE(m_widget));
    } 
}

void
GtkTextImpl::OnCopy()
{
    if (m_widget) {
        gtk_editable_copy_clipboard(GTK_EDITABLE(m_widget));
    } 
}

void
GtkTextImpl::OnPaste()
{
    if (m_widget) {
        gtk_editable_paste_clipboard(GTK_EDITABLE(m_widget));
    } 
}

void
GtkTextImpl::OnClear()
{
    if (m_widget) {
        gtk_editable_delete_text(GTK_EDITABLE(m_widget), 0, -1);
    } 
}

void
GtkTextImpl::OnSelectAll()
{
    if (m_widget) {
        gtk_editable_select_region(GTK_EDITABLE(m_widget), 0, -1);
    } 
}

