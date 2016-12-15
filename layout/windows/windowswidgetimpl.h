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

#if !defined(__WINDOWSWIDGETIMPL_H__)
#define __WINDOWSWIDGETIMPL_H__

#include "prtypes.h"
#include "../widgetimpl.h"

#include "stdafx.h"

// for discussion of gcroot.h, see 
// http://www.ondotnet.com/pub/a/dotnet/2003/03/03/mcppp2.html?page=2

#include <gcroot.h>

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

#include "formimpl.h"
#include "netscrolledviewimpl.h"
#include "netscrolledwindowimpl.h"

#include "controlgotfocushelper.h"

class WindowsWidgetImpl 
{
public:
	WindowsWidgetImpl() : m_form(NULL), m_sv(NULL), m_sw(NULL),
        m_formParent(NULL), m_svParent(NULL), m_swParent(NULL) {};
    virtual ~WindowsWidgetImpl() {};
    PRStatus ShowImpl();
    PRStatus HideImpl();
    PRStatus DisableImpl();
    PRStatus EnableImpl();
    PRStatus GetGeometry(int &x, int &y, int &width, int &height);
    PRStatus SetGeometryImpl(const int &x, const int &y, 
        const int &width, const int &height, const char &mask);
    PRStatus SetWidgetParent(WidgetImpl *top);

    PRStatus SetForm(FormImpl *form);
    FormImpl *GetForm();
    PRStatus SetFormParent(WidgetImpl *top);

    PRStatus SetScrolledView(NetScrolledViewImpl *sv);
    NetScrolledViewImpl *GetScrolledView();
    PRStatus SetScrolledViewParent(WidgetImpl *top);

    PRStatus SetScrolledWindow(NetScrolledWindowImpl *sv);
    NetScrolledWindowImpl *GetScrolledWindow();
    PRStatus SetScrolledWindowParent(WidgetImpl *top);

private:
    gcroot<FormImpl *> m_form;  // NULL for anything but a WindowImpl
    gcroot<NetScrolledViewImpl *> m_sv;  // NULL except for a ScrolledViewImpl
    gcroot<NetScrolledWindowImpl *> m_sw;  // NULL except for a ScrolledWindowImpl
protected:
    gcroot<FormImpl *> m_formParent;
    gcroot<NetScrolledViewImpl *> m_svParent;
    gcroot<NetScrolledWindowImpl *> m_swParent;
    gcroot<ControlGotFocusHelper *> m_gotFocusHelper;
};
#endif
