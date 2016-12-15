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

#include "windowswidgetimpl.h"
#include "../geometry.h"
#include "../widgetimpl.h"

ControlGotFocusHelper::ControlGotFocusHelper(WidgetImpl *impl)
{
    m_impl = impl;
}

void ControlGotFocusHelper::OnGotFocusHandler(Object *sender, System::EventArgs*
 e)
{
    if (m_impl && sender) {
        Document *doc = m_impl->GetDocument();
        if (doc)
            doc->SetActive(m_impl->GetWidget());
    }
}

PRStatus WindowsWidgetImpl::SetGeometryImpl(const int &x, const int &y,
    const int &width, const int &height, const char &mask)
{
    return PR_FAILURE;
}

PRStatus WindowsWidgetImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    return PR_FAILURE;
}

PRStatus WindowsWidgetImpl::EnableImpl()
{
    return PR_FAILURE;
}

PRStatus WindowsWidgetImpl::DisableImpl()
{
    return PR_FAILURE;
}

PRStatus WindowsWidgetImpl::ShowImpl()
{
    return PR_FAILURE;
}

PRStatus WindowsWidgetImpl::HideImpl()
{
    return PR_FAILURE;
}

PRStatus WindowsWidgetImpl::SetForm(FormImpl *form)
{
    m_form = form;
    return PR_SUCCESS;
}

FormImpl * WindowsWidgetImpl::GetForm()
{
    return m_form;
}

PRStatus WindowsWidgetImpl::SetScrolledView(NetScrolledViewImpl *sv)
{
    m_sv = sv;
    return PR_SUCCESS;
}

NetScrolledViewImpl * WindowsWidgetImpl::GetScrolledView()
{
    return m_sv;
}

PRStatus WindowsWidgetImpl::SetScrolledWindow(NetScrolledWindowImpl *sw)
{
    m_sw = sw;
    return PR_SUCCESS;
}

NetScrolledWindowImpl * WindowsWidgetImpl::GetScrolledWindow()
{
    return m_sw;
}

PRStatus WindowsWidgetImpl::SetWidgetParent(WidgetImpl *top)
{
    PRStatus ret = PR_FAILURE;

    WindowsWidgetImpl *wImpl = dynamic_cast<WindowsWidgetImpl *>(top);

    if (wImpl && wImpl->GetForm()) {
        SetFormParent(top);
        ret = PR_SUCCESS;
    } else if (wImpl && wImpl->GetScrolledView()) {
        SetScrolledViewParent(top);
        ret = PR_SUCCESS;
    } else if (wImpl && wImpl->GetScrolledWindow()) {
        SetScrolledWindowParent(top);
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus WindowsWidgetImpl::SetFormParent(WidgetImpl *top)
{
    if (top) {
        WindowsWidgetImpl *widgetImpl = dynamic_cast<WindowsWidgetImpl *>(top);

        if (widgetImpl) {
            FormImpl *form = widgetImpl->GetForm();
            if (form) {
                m_formParent = form;
                return PR_SUCCESS;
            } 
        }
    }
    return PR_FAILURE;
}

PRStatus WindowsWidgetImpl::SetScrolledViewParent(WidgetImpl *top)
{
    if (top) {
        WindowsWidgetImpl *widgetImpl = dynamic_cast<WindowsWidgetImpl *>(top);

        if (widgetImpl) {
            NetScrolledViewImpl *sv = widgetImpl->GetScrolledView();
            if (sv) {
                m_svParent = sv;
                return PR_SUCCESS;
            } 
        }
    }
    return PR_FAILURE;
}

PRStatus WindowsWidgetImpl::SetScrolledWindowParent(WidgetImpl *top)
{
    if (top) {
        WindowsWidgetImpl *widgetImpl = dynamic_cast<WindowsWidgetImpl *>(top);

        if (widgetImpl) {
            NetScrolledWindowImpl *sw = widgetImpl->GetScrolledWindow();
            if (sw) {
                m_swParent = sw;
                return PR_SUCCESS;
            } 
        }
    }
    return PR_FAILURE;
}

