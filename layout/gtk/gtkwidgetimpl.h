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

#if !defined(__GTKWIDGETIMPL_H__)
#define __GTKWIDGETIMPL_H__

#include <gtk/gtk.h>
#include "prtypes.h"
#include "../widgetimpl.h"

class GtkWidgetImpl 
{
public:
	GtkWidgetImpl() : m_widget(NULL), m_fixedParent(NULL) {};
    virtual ~GtkWidgetImpl() {if (m_widget && GTK_IS_WIDGET(m_widget)) {gtk_widget_destroy(GTK_WIDGET(m_widget));} m_widget = NULL;};
    PRStatus ShowImpl();
    PRStatus HideImpl();
    PRStatus EnableImpl();
    PRStatus DisableImpl();
    PRStatus GetGeometryRequestImpl(int &x, int &y, 
        int &width, int &height);
    PRStatus SetGeometryImpl(const int &x, const int &y, 
        const int &width, const int &height, const char &mask);
    virtual GtkWidget *GetImpl() {return m_widget;};
    PRStatus SetFixedParent(WidgetImpl *top);
protected:
    void RegisterFocusCallback();
    GtkWidget *m_widget;
    GtkWidget *m_fixedParent;   // cached GtkFixed parent of all children
};
#endif
