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
#include "gtkscrolledviewimpl.h"

GtkScrolledViewImpl::GtkScrolledViewImpl() : m_scrolledWindow(NULL) 
{
}

GtkScrolledViewImpl::~GtkScrolledViewImpl()
{
	// destroy the scrolledview
}

PRStatus GtkScrolledViewImpl::Create()
{
    // create the scrolledview

    m_widget = gtk_fixed_new();

#if 0
    if (m_widget) {

#if 0
        // set the scrollbar step sizes to 10 pixels
        // XXX this should be configurable
 
        GTK_LAYOUT(m_widget)->hadjustment->step_increment = 10.0;
        GTK_LAYOUT(m_widget)->vadjustment->step_increment = 10.0;
#endif

        // find the containing scrolledwindow widget

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
#else
    return PR_SUCCESS;
#endif
}

PRStatus GtkScrolledViewImpl::Attach(ScrolledWindowImpl *impl)
{
    PRStatus ret = PR_FAILURE;
    if (impl) {
        GtkWidgetImpl *wImpl = dynamic_cast<GtkWidgetImpl *>(impl);
        if (wImpl) {
            m_scrolledWindow = dynamic_cast<GtkScrolledWindowImpl *>(wImpl);
            GtkWidget *w = wImpl->GetImpl();
            if (w) {
                gtk_scrolled_window_add_with_viewport(
                    GTK_SCROLLED_WINDOW(w), m_widget);
                ret = PR_SUCCESS;
            }
        }
    }
    return ret;
}

PRStatus
GtkScrolledViewImpl::Show()
{
    return ShowImpl();
}

PRStatus
GtkScrolledViewImpl::Hide()
{
    return HideImpl();
}

PRStatus
GtkScrolledViewImpl::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char &mask)
{
    if (m_scrolledWindow)
        m_scrolledWindow->SetAdjustment(width, height);
/*
    if (m_widget) {
        GtkAdjustment *vadj, *hadj;

        vadj = (GtkAdjustment *) 
            gtk_adjustment_new(0.0, 0.0, height - 1.0, 1.0, 1.0, 0.0);
        hadj = (GtkAdjustment *) 
            gtk_adjustment_new(0.0, 0.0, width - 1.0, 1.0, 1.0, 0.0);
        gtk_scrolled_window_set_hadjustment(GTK_SCROLLED_WINDOW(m_widget),
            hadj);
        gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(m_widget),
            vadj);
    }
*/
    return SetGeometryImpl(x, y, width, height, mask); 
}

PRStatus
GtkScrolledViewImpl::Enable()
{
    return EnableImpl();
}

PRStatus
GtkScrolledViewImpl::Disable()
{
    return DisableImpl();
}
