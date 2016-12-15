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
#include "gtkscrolledwindowimpl.h"

GtkScrolledWindowImpl::GtkScrolledWindowImpl() : m_hadj(NULL), m_vadj(NULL)
{
}

GtkScrolledWindowImpl::~GtkScrolledWindowImpl()
{
	// destroy the scrolledwindow
}

PRStatus
GtkScrolledWindowImpl::SetAdjustment(const int &width, const int &height)
{
    if (m_widget && m_vadj && m_hadj) {
        GTK_ADJUSTMENT(m_vadj)->upper = height - 1.0;
        GTK_ADJUSTMENT(m_hadj)->upper = width - 1.0;
        gtk_adjustment_changed(m_hadj);
        gtk_adjustment_changed(m_vadj);
    }
    return PR_SUCCESS; 
}

PRStatus GtkScrolledWindowImpl::Create()
{
	// create the scrolledwindow

	m_widget = gtk_scrolled_window_new(NULL, NULL); 

	if (m_widget) {

        m_vadj = (GtkAdjustment *) 
            gtk_adjustment_new(0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
        m_hadj = (GtkAdjustment *) 
            gtk_adjustment_new(0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
        gtk_scrolled_window_set_hadjustment(GTK_SCROLLED_WINDOW(m_widget),
            m_hadj);
        gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(m_widget),
            m_vadj);

        if (!m_fixedParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetFixedParent(top);
            }
        }

        if (m_fixedParent) {
            gtk_fixed_put(GTK_FIXED(m_fixedParent), m_widget, 0, 0);
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus
GtkScrolledWindowImpl::Show()
{
    return ShowImpl();
}

PRStatus
GtkScrolledWindowImpl::Hide()
{
    return HideImpl();
}

PRStatus
GtkScrolledWindowImpl::Enable()
{
    return EnableImpl();
}

PRStatus
GtkScrolledWindowImpl::Disable()
{
    return DisableImpl();
}
