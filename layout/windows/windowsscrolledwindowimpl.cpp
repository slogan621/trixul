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
#include "windowsscrolledwindowimpl.h"

WindowsScrolledWindowImpl::WindowsScrolledWindowImpl() : m_scrolledWindow(NULL)
{
}

WindowsScrolledWindowImpl::~WindowsScrolledWindowImpl()
{
}

PRStatus WindowsScrolledWindowImpl::Create()
{
	// create the scrolledwindow

	m_scrolledWindow = __gc new NetScrolledWindowImpl(); 

	if (m_scrolledWindow) {
        SetScrolledWindow(m_scrolledWindow);

        m_scrolledWindow->AutoScroll = true;
        if (!m_formParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetFormParent(top);
            }
        }

        if (m_formParent) {
            m_formParent->Controls->Add(m_scrolledWindow);
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus 
WindowsScrolledWindowImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    Size size, request(0, 0);

    if (m_scrolledWindow) {
        size = m_scrolledWindow->ClientSize;
        width = size.get_Width();
        height = size.get_Height();
    }
    return PR_SUCCESS;
}

PRStatus 
WindowsScrolledWindowImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    if (m_scrolledWindow) {
        Size size;
        Point point;

        size = m_scrolledWindow->Size;
        point = m_scrolledWindow->Location;

        if (mask & GEOM_X)
            point.X = x;
        if (mask & GEOM_Y)
            point.Y = y;
        if (mask & GEOM_WIDTH)
            size.Width = width;
        if (mask & GEOM_HEIGHT)
            size.Height = height;

        m_scrolledWindow->Size=size;
        m_scrolledWindow->Location=point;
    }

    return PR_SUCCESS;
}

PRStatus
WindowsScrolledWindowImpl::Show()
{
    if (m_scrolledWindow) {
        m_scrolledWindow->Show();
        m_scrolledWindow->Visible = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsScrolledWindowImpl::Hide()
{
    if (m_scrolledWindow) {
        m_scrolledWindow->Hide();
        m_scrolledWindow->Visible = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsScrolledWindowImpl::Enable()
{
    if (m_scrolledWindow) {
        m_scrolledWindow->Enabled = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsScrolledWindowImpl::Disable()
{
    if (m_scrolledWindow) {
        m_scrolledWindow->Enabled = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}
