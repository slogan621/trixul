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
#include "cocoascrolledviewimpl.h"

CocoaScrolledViewImpl::CocoaScrolledViewImpl()
{
	m_scrolledView = NULL;
}

CocoaScrolledViewImpl::~CocoaScrolledViewImpl()
{
    if (m_scrolledView)
        [m_scrolledView release];
}

PRStatus CocoaScrolledViewImpl::Create()
{
    NSRect graphicsRect = NSMakeRect(1.0, 1.0, 1.0, 1.0);

	// create the scrolledview

	m_scrolledView = [[NSView alloc] initWithFrame:graphicsRect];

	if (m_scrolledView) {
        m_view = (NSView *) m_scrolledView;
#if 1
		WidgetImpl *parentImpl = GetParent();

		if (parentImpl) {

            NSView *parentView =
                dynamic_cast<CocoaWidgetImpl *>(parentImpl)->GetView();

			// add the scrolledview to the parent view (a scrolled window)

			if (parentView) {
				[parentView addSubview: m_view];
				return PR_SUCCESS;
			}
		}
#endif
	}

    return PR_FAILURE;
}

PRStatus CocoaScrolledViewImpl::Attach(ScrolledWindowImpl *impl)
{
    PRStatus ret = PR_FAILURE;
    if (impl) {
        CocoaWidgetImpl *wImpl = dynamic_cast<CocoaWidgetImpl *>(impl);
        if (wImpl) {
            NSScrollView *view = wImpl->GetView();
            if (view) {
                [view setDocumentView: m_view];
                ret = PR_SUCCESS;
            }
        }
    }
    return ret;
}

PRStatus 
CocoaScrolledViewImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_scrolledView) {
        NSRect graphicsRect;

        graphicsRect = [m_scrolledView frame];
        x = static_cast<int>(graphicsRect.origin.x);
        y = static_cast<int>(graphicsRect.origin.y);
        width = static_cast<int>(graphicsRect.size.width);
        height = static_cast<int>(graphicsRect.size.height);
        return PR_SUCCESS;
    }

    return PR_FAILURE;
}

PRStatus 
CocoaScrolledViewImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    int newY = AdjustY(y, height);

    if (m_scrolledView) {
        NSRect graphicsRect;
        if (mask == GEOM_ALL)
            graphicsRect = NSMakeRect(x, newY, width, height);
        else {
            graphicsRect = [m_scrolledView frame];
            if (mask & GEOM_X)
                graphicsRect.origin.x = x;
            if (mask & GEOM_Y)
                graphicsRect.origin.y = newY;
            if (mask & GEOM_WIDTH)
                graphicsRect.size.width = width;
            if (mask & GEOM_HEIGHT)
                graphicsRect.size.height = height;
        }

        [m_scrolledView setFrame: graphicsRect];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
CocoaScrolledViewImpl::Show()
{
    return ShowImpl();
}

PRStatus
CocoaScrolledViewImpl::Hide()
{
    return HideImpl();
}

PRStatus
CocoaScrolledViewImpl::Enable()
{
    return EnableImpl();
}

PRStatus
CocoaScrolledViewImpl::Disable()
{
    return DisableImpl();
}
