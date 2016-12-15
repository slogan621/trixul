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
#include "gtkbuttonimpl.h"
#include "gtkbuttonaction.h"

GtkButtonImpl::GtkButtonImpl() 
{
}

GtkButtonImpl::~GtkButtonImpl()
{
}

extern "C" {

gint HandleCommandThunk(GtkWidget *widget, gpointer callbackData)
{
    GtkButtonImpl *pButtonImpl = (GtkButtonImpl *) callbackData;
    if (pButtonImpl)
        pButtonImpl->HandleCommand();
    return TRUE;
}

} // extern "C"

PRStatus GtkButtonImpl::HandleCommand()
{
    // notify the document so it can recompute layout

    NotifyButtonPress();
    return PR_SUCCESS;
}

PRStatus GtkButtonImpl::Create()
{
    m_widget = gtk_button_new();
    if (m_widget) {
       
        if (!m_fixedParent) { 
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetFixedParent(top);
            }
        }

        if (m_fixedParent) {
            gtk_fixed_put(GTK_FIXED(m_fixedParent), m_widget, 0, 0);
            gtk_signal_connect(GTK_OBJECT(m_widget), "clicked",
                GTK_SIGNAL_FUNC(HandleCommandThunk), this);
            return PR_SUCCESS;
        } 
    }
	return PR_FAILURE;
}

PRStatus GtkButtonImpl::SetLabel(const string& label)
{
    if (m_widget) {
        gtk_object_set(GTK_OBJECT(m_widget),
            "GtkButton::label", label.c_str(), NULL);
        return PR_SUCCESS;
    }
	return PR_FAILURE;
}

PRStatus
GtkButtonImpl::Show()
{
    return ShowImpl();
}

PRStatus
GtkButtonImpl::Hide()
{
    return HideImpl();
}

PRStatus
GtkButtonImpl::Enable()
{
    return EnableImpl();
}       
        
PRStatus    
GtkButtonImpl::Disable()
{           
    return DisableImpl();
}               

