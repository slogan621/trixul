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
#include "gtkradiobuttonimpl.h"

#include "../radiogroupmanager.h"

GtkRadioButtonImpl::GtkRadioButtonImpl() 
{
}

GtkRadioButtonImpl::~GtkRadioButtonImpl()
{
	// destroy the radiobutton
    RadioGroupManager *mgr = RadioGroupManager::GetManager(GetDocument());
    if (mgr)
        mgr->RemoveFromGroup(m_group, this);

}

PRStatus GtkRadioButtonImpl::Create()
{
    m_widget = gtk_radio_button_new(NULL);
    if (m_widget) {
        SetChecked(false);
        if (!m_fixedParent) { 
            WidgetImpl *top = GetRootWidget();
            if (top) 
                SetFixedParent(top);
        }

        if (m_fixedParent) {
            gtk_fixed_put(GTK_FIXED(m_fixedParent), m_widget, 0, 0);
            return PR_SUCCESS;
        } 
    }
	return PR_FAILURE;
}

PRStatus GtkRadioButtonImpl::SetLabel(const string& label)
{
    if (m_widget) {
        gtk_object_set(GTK_OBJECT(m_widget),
            "GtkButton::label", label.c_str(), NULL);
        return PR_SUCCESS;
    }
	return PR_FAILURE;
}

PRStatus
GtkRadioButtonImpl::Show()
{
    return ShowImpl();
}

PRStatus
GtkRadioButtonImpl::Hide()
{
    return HideImpl();
}

PRStatus
GtkRadioButtonImpl::Enable()
{
    return EnableImpl();
}       
        
PRStatus    
GtkRadioButtonImpl::Disable()
{           
    return DisableImpl();
}               

PRStatus 
GtkRadioButtonImpl::SetChecked(const bool checked)
{
    PRStatus ret = PR_FAILURE;
    if (m_widget) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_widget), checked);
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus 
GtkRadioButtonImpl::GetChecked(bool &checked)
{
    PRStatus ret = PR_FAILURE;
    if (m_widget) {
        checked =  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_widget));
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus 
GtkRadioButtonImpl::SetGroup(const string& group)
{
    PRStatus ret = PR_FAILURE;
    if (m_widget) {
        m_group = group;
        RadioGroupManager *mgr = RadioGroupManager::GetManager(GetDocument());
        if (mgr) {
            int size = mgr->GetGroupSize(group);
            if (size == 0) {
                gtk_radio_button_set_group(GTK_RADIO_BUTTON(m_widget), NULL);
                ret = PR_SUCCESS;
            } else {
                GtkWidgetImpl *pImpl = 
                    dynamic_cast<GtkWidgetImpl *>(mgr->GetFirstItem(group));
                if (pImpl) {
                    GtkWidget *w = pImpl->GetImpl();
                    if (w) {
                        gtk_radio_button_set_group(GTK_RADIO_BUTTON(m_widget),
                            gtk_radio_button_group(GTK_RADIO_BUTTON(w)));
                        ret = PR_SUCCESS;
                    }
                } 
            }
            mgr->AddToGroup(m_group, this);
        }
    }
    return ret;
}

