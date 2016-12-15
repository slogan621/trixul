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

#include "gtkgridlistimpl.h"

GtkGridListImpl::GtkGridListImpl() : m_clist(NULL), m_rowCount(0)
{
}

GtkGridListImpl::~GtkGridListImpl()
{
}

extern "C" {

gint HandleSelectionThunk(GtkWidget *widget, gint row, gint col, 
    GdkEventButton *event, gpointer callbackData)
{
    GtkGridListImpl *pGridListImpl = (GtkGridListImpl *) callbackData;
    if (pGridListImpl)
        pGridListImpl->Select(row);
    return TRUE;
}

gint HandleDeselectionThunk(GtkWidget *widget, gint row, gint col, 
    GdkEventButton *event, gpointer callbackData)
{
    GtkGridListImpl *pGridListImpl = (GtkGridListImpl *) callbackData;
    if (pGridListImpl)
        pGridListImpl->Deselect(row);
    return TRUE;
}

} // extern "C"

PRStatus GtkGridListImpl::Create()
{
    m_widget = gtk_scrolled_window_new(NULL, NULL);
    if (m_widget) {
        m_clist = gtk_clist_new(1);
        m_selections.empty();
        if (m_clist) {
            gtk_container_add(GTK_CONTAINER(m_widget), m_clist);
            gtk_clist_set_selection_mode(GTK_CLIST(m_clist), 
                GTK_SELECTION_MULTIPLE);
            
            if (!m_fixedParent) {
                WidgetImpl *top = GetRootWidget();
                if (top) {
                    SetFixedParent(top);
                }
            }

            if (m_fixedParent) {
                gtk_fixed_put(GTK_FIXED(m_fixedParent), m_widget, 0, 0);
                gtk_signal_connect(GTK_OBJECT(m_clist), "select_row",
                    GTK_SIGNAL_FUNC(HandleSelectionThunk), this);
                gtk_signal_connect(GTK_OBJECT(m_clist), "unselect_row",
                    GTK_SIGNAL_FUNC(HandleDeselectionThunk), this);
                return PR_SUCCESS;
            }
        }
    }
    return PR_FAILURE;
}

PRStatus
GtkGridListImpl::Show()
{
    if (m_clist)
        gtk_widget_show(m_clist);
    return ShowImpl();
}

PRStatus
GtkGridListImpl::Hide()
{
    if (m_clist)
        gtk_widget_hide(m_clist);
    return HideImpl();
}

PRStatus
GtkGridListImpl::Enable()
{
    if (m_clist)
        gtk_widget_set_sensitive(m_clist, true);
    return EnableImpl();
}

PRStatus
GtkGridListImpl::Disable()
{
    if (m_clist)
        gtk_widget_set_sensitive(m_clist, false);
    return DisableImpl();
}

PRStatus GtkGridListImpl::GetSelectionCount(int &count)
{
    count = m_selections.size();
    return PR_SUCCESS;
}

PRStatus GtkGridListImpl::GetSelection(const int index, string &value)
{
    PRStatus ret = PR_FAILURE;
    if (m_clist) {
        list <int>::iterator iter;
        int i = 0;
        for (iter = m_selections.begin(); iter != m_selections.end(); ++iter) {
            if (i == index) {
                char *text = NULL;
                if (gtk_clist_get_text(GTK_CLIST(m_clist), (*iter), 0, &text)) {
                    value = text;
                    ret = PR_SUCCESS;
                    break;
                }
            }
            i++;
        }
    }
    return ret;
}

PRStatus GtkGridListImpl::RemoveItemByValue(const string &value)
{
    PRStatus ret = PR_FAILURE;
    if (m_clist) {
        Clear();
        for (int i = 0; i < m_rowCount; i++) {
            char *text;
            if (gtk_clist_get_text(GTK_CLIST(m_clist), i, 0, &text))
                return RemoveItemByPosition(i+1);
        }
    }
    return ret;
}

PRStatus GtkGridListImpl::RemoveItemByPosition(const int position)
{
printf("GtkGridListImpl::RemoveByPosition %d\n", position);
    PRStatus ret = PR_FAILURE;
    if (m_clist && position > 0 && position <= m_rowCount) {
        Clear();
        gtk_clist_remove(GTK_CLIST(m_clist), position - 1); 
        m_rowCount--;
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus GtkGridListImpl::AddItem(const string &value, const int position)
{
    PRStatus ret = PR_FAILURE;
    if (m_clist) {
        char *val[2];
        val[0] = const_cast<char *>(value.c_str());
        val[1] = NULL;
        gtk_clist_insert(GTK_CLIST(m_clist), position, val);
        Clear();
        m_rowCount++;
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus GtkGridListImpl::AppendItem(const string &value)
{
    PRStatus ret = PR_FAILURE;
    if (m_clist) {
        char *val[2];
        val[0] = const_cast<char *>(value.c_str());
        val[1] = NULL;
        gtk_clist_append(GTK_CLIST(m_clist), val);
        Clear();
        m_rowCount++;
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus GtkGridListImpl::Select(int row)
{
    if (m_selections.size() > 0) {
        list <int>::iterator iter;
        iter = find (m_selections.begin(), m_selections.end(), row);
        if (iter == m_selections.end())
            m_selections.push_back(row);
    } else {
        m_selections.push_back(row);
    }
    return PR_SUCCESS;
}

PRStatus GtkGridListImpl::Deselect(int row)
{
    if (m_selections.size() > 0) {
        list <int>::iterator iter;
        iter = find (m_selections.begin(), m_selections.end(), row);
        if (iter != m_selections.end())
            m_selections.remove(row);
    }
    return PR_SUCCESS;
}

PRStatus GtkGridListImpl::Clear()
{
    if (m_widget) {
        list <int>::iterator iter;
        for (iter = m_selections.begin(); iter != m_selections.end(); ++iter)
            gtk_clist_unselect_row(GTK_CLIST(m_clist), (*iter), -1);
    }
    m_selections.empty();
    return PR_SUCCESS;
}

