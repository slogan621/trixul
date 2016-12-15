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

#include "cocoawindowimpl.h"
#include "cocoascrolledwindowimpl.h"

CocoaScrolledWindowImpl::CocoaScrolledWindowImpl()
{
	m_scrolledWindow = NULL;
}

CocoaScrolledWindowImpl::~CocoaScrolledWindowImpl()
{
    if (m_scrolledWindow)
        [m_scrolledWindow release];
}

PRStatus CocoaScrolledWindowImpl::Create()
{
    NSRect graphicsRect = NSMakeRect(1.0, 1.0, 1.0, 1.0);

	// create the scrolledwindow

	m_scrolledWindow = [[NSScrollView alloc] initWithFrame:graphicsRect];

	if (m_scrolledWindow) {
        NSSize contentSize = [m_scrolledWindow contentSize];
        [m_scrolledWindow setBorderType:NSNoBorder];
        [m_scrolledWindow setHasVerticalScroller:YES];
        [m_scrolledWindow setHasHorizontalScroller:YES];
        [m_scrolledWindow setAutoresizingMask:NSViewWidthSizable |
            NSViewHeightSizable];

		WidgetImpl *parentImpl = GetParent();
        m_view = (NSView *) m_scrolledWindow;

		if (parentImpl) {

			NSView *parentView = 
                dynamic_cast<CocoaWidgetImpl *>(parentImpl)->GetView();

			// add the scrolledwindow to the parent view

			if (parentView) {
				[parentView addSubview: m_view];
				return PR_SUCCESS;
			}
		}
	}

    return PR_FAILURE;
}

PRStatus 
CocoaScrolledWindowImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_scrolledWindow) {
        NSRect graphicsRect;

        graphicsRect = [m_scrolledWindow frame];
        x = static_cast<int>(graphicsRect.origin.x);
        y = static_cast<int>(graphicsRect.origin.y);
        width = static_cast<int>(graphicsRect.size.width);
        height = static_cast<int>(graphicsRect.size.height);
        return PR_SUCCESS;
    }

    return PR_FAILURE;
}

PRStatus 
CocoaScrolledWindowImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    int newY = AdjustY(y, height);

    if (m_scrolledWindow) {
        NSRect graphicsRect;
        if (mask == GEOM_ALL)
            graphicsRect = NSMakeRect(x, newY, width, height);
        else {
            graphicsRect = [m_scrolledWindow frame];
            if (mask & GEOM_X)
                graphicsRect.origin.x = x;
            if (mask & GEOM_Y)
                graphicsRect.origin.y = newY;
            if (mask & GEOM_WIDTH)
                graphicsRect.size.width = width;
            if (mask & GEOM_HEIGHT)
                graphicsRect.size.height = height;
        }

        [m_scrolledWindow setFrame: graphicsRect];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
CocoaScrolledWindowImpl::Show()
{
    return ShowImpl();
}

PRStatus
CocoaScrolledWindowImpl::Hide()
{
    return HideImpl();
}

PRStatus
CocoaScrolledWindowImpl::Enable()
{
    return EnableImpl();
}

PRStatus
CocoaScrolledWindowImpl::Disable()
{
    return DisableImpl();
}
