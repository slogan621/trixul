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

#include "gtkwidgetimpl.h"
#include "../geometry.h"
#include "../widgetimpl.h"

PRStatus GtkWidgetImpl::SetGeometryImpl(const int &x, const int &y,
    const int &width, const int &height, const char &mask)
{
    if (m_widget) {
        // get the current values

        GtkArg arg;

        if (mask & GEOM_X) {
            arg.name = "GtkWidget::x";
            arg.type = GTK_TYPE_INT;
            GTK_VALUE_INT(arg) = x;
            gtk_object_arg_set(GTK_OBJECT(m_widget), &arg, NULL);
        }

        if (mask & GEOM_Y) {
            arg.name = "GtkWidget::y";
            arg.type = GTK_TYPE_INT;
            GTK_VALUE_INT(arg) = y;
            gtk_object_arg_set(GTK_OBJECT(m_widget), &arg, NULL);
        }

        if (mask & GEOM_WIDTH) {
            arg.name = "GtkWidget::width";
            arg.type = GTK_TYPE_INT;
            GTK_VALUE_INT(arg) = width;
            gtk_object_arg_set(GTK_OBJECT(m_widget), &arg, NULL);
        }

        if (mask & GEOM_HEIGHT) {
            arg.name = "GtkWidget::height";
            arg.type = GTK_TYPE_INT;
            GTK_VALUE_INT(arg) = height;
            gtk_object_arg_set(GTK_OBJECT(m_widget), &arg, NULL);
        }

        return PR_SUCCESS;
    }
        
    return PR_FAILURE;
}

PRStatus GtkWidgetImpl::GetGeometryRequestImpl(int &x, int &y, 
    int &width, int &height)
{
    GtkRequisition requisition;

    if (m_widget) {
        gtk_widget_size_request(m_widget, &requisition);
        width = requisition.width;
        height = requisition.height;
        return PR_SUCCESS;
    }
        
    return PR_FAILURE;
}

PRStatus GtkWidgetImpl::ShowImpl()
{
    if (m_widget) {
        gtk_widget_show(m_widget);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus GtkWidgetImpl::HideImpl()
{
    if (m_widget) {
        gtk_widget_hide(m_widget);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus GtkWidgetImpl::DisableImpl()
{
    if (m_widget) {
        gtk_widget_set_sensitive(m_widget, false);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus GtkWidgetImpl::EnableImpl()
{
    if (m_widget) {
        gtk_widget_set_sensitive(m_widget, true);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus GtkWidgetImpl::SetFixedParent(WidgetImpl *top)
{
    if (top) {
        GtkWidgetImpl *widgetImpl = dynamic_cast<GtkWidgetImpl *>(top);

        if (widgetImpl) {
            GtkWidget *w = widgetImpl->GetImpl();
            if (w) {
                m_fixedParent = w;
                return PR_SUCCESS;
            }
        }
    }
    return PR_FAILURE;
}

extern "C" {
void
ActivateCallback(GtkWidget *w, gpointer data)
{
    WidgetImpl *wImpl = (WidgetImpl *) data;

    if (wImpl) {
        Document *doc = wImpl->GetDocument();
        if (doc)
            doc->SetActive(wImpl->GetWidget());
    }
}
}

void
GtkWidgetImpl::RegisterFocusCallback()
{
    gtk_signal_connect(GTK_OBJECT(m_widget), "grab_focus",
        GTK_SIGNAL_FUNC(ActivateCallback), dynamic_cast<WidgetImpl *>(this));
}
