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

#include "windowswindowimpl.h"
#include "windowsscrolledviewimpl.h"

WidgetImpl *WindowsScrolledViewImpl::GetRootWidget()
{
    WidgetImpl *pImpl = NULL, *qImpl = NULL;
    ElementType type;

    pImpl = GetParent();
    while (pImpl) {
        qImpl = pImpl;
        type = pImpl->GetType();
        if (type == TYPE_WINDOW || type == TYPE_SCROLLEDVIEW || type == TYPE_SCROLLEDWINDOW) {
            break;
        }
        pImpl = pImpl->GetParent();
    }
    return qImpl;
}

WindowsScrolledViewImpl::WindowsScrolledViewImpl() : m_scrolledView(NULL)
{
}

WindowsScrolledViewImpl::~WindowsScrolledViewImpl()
{
}

PRStatus WindowsScrolledViewImpl::Create()
{
	// create the scrolledview

	m_scrolledView = __gc new NetScrolledViewImpl(); 

	if (m_scrolledView) {
        SetScrolledView(m_scrolledView);
        if (!m_formParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetWidgetParent(top);
            }
        }

        bool added = false;
        if (m_formParent) {
            m_formParent->Controls->Add(m_scrolledView);
            added = true;
        } else if (m_svParent) {
            m_svParent->Controls->Add(m_scrolledView);
            added = true;
        } else if (m_swParent) {
            m_swParent->Controls->Add(m_scrolledView);
            added = true;
        }

        if (added)
            return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus 
WindowsScrolledViewImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    Size size, request(0, 0);

    if (m_scrolledView) {
        size = m_scrolledView->ClientSize;
        width = size.get_Width();
        height = size.get_Height();
    }
    return PR_SUCCESS;
}

PRStatus 
WindowsScrolledViewImpl::Attach(ScrolledWindowImpl *impl)
{
    return PR_SUCCESS;
}

PRStatus 
WindowsScrolledViewImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    if (m_scrolledView) {

        Size size;
        Point point;

        size = m_scrolledView->Size;
        point = m_scrolledView->Location;


        if (mask & GEOM_X)
            point.X = x;
        if (mask & GEOM_Y)
            point.Y = y;
        if (mask & GEOM_WIDTH)
            size.Width = width;
        if (mask & GEOM_HEIGHT)
            size.Height = height;

        m_scrolledView->Size=size;
        m_scrolledView->Location=point;
    }

    return PR_SUCCESS;
}

PRStatus
WindowsScrolledViewImpl::Show()
{
    if (m_scrolledView) {
        m_scrolledView->Show();
        m_scrolledView->Visible = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsScrolledViewImpl::Hide()
{
    if (m_scrolledView) {
        m_scrolledView->Hide();
        m_scrolledView->Visible = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsScrolledViewImpl::Enable()
{
    if (m_scrolledView) {
        m_scrolledView->Enabled = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsScrolledViewImpl::Disable()
{
    if (m_scrolledView) {
        m_scrolledView->Enabled = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}
