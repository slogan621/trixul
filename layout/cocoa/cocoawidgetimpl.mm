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

#include "cocoawidgetimpl.h"
#include "cocoawindowimpl.h"
#include "../geometry.h"
#include "../widgetimpl.h"

PRStatus CocoaWidgetImpl::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char &mask)
{
    int newY = AdjustY(y, height);

    if (m_view) {
        NSRect graphicsRect;
        if (mask == GEOM_ALL)
            graphicsRect = NSMakeRect(x, newY, width, height);
        else {
            graphicsRect = [m_view frame];
            if (mask & GEOM_X)
                graphicsRect.origin.x = x;
            if (mask & GEOM_Y)
                graphicsRect.origin.y = newY;
            if (mask & GEOM_WIDTH)
                graphicsRect.size.width = width;
            if (mask & GEOM_HEIGHT)
                graphicsRect.size.height = height;
        }

        [m_view setFrame: graphicsRect];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus CocoaWidgetImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_view) {
        NSRect graphicsRect;

        graphicsRect = [m_view frame];
        x = static_cast<int>(graphicsRect.origin.x);
        y = static_cast<int>(graphicsRect.origin.y);
        width = static_cast<int>(graphicsRect.size.width);
        height = static_cast<int>(graphicsRect.size.height);

        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus CocoaWidgetImpl::DisableImpl()
{
    if (m_view) {
        NSControl *control = (NSControl *) m_view;
        [control setEnabled:NO];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus CocoaWidgetImpl::EnableImpl()
{
    if (m_view) {
        NSControl *control = (NSControl *) m_view;
        [control setEnabled:YES];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus CocoaWidgetImpl::ShowImpl()
{
    // show the widget 

    if (m_view) {
        [m_view setHidden: NO];
        [m_view display];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus CocoaWidgetImpl::HideImpl()
{
    // show the widget 

    if (m_view) {
        [m_view setHidden: YES];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

int CocoaWidgetImpl::AdjustY(const int y, const int height)
{
    NSView *nsView;
    int newY = y;
    WidgetImpl *wImpl = dynamic_cast<WidgetImpl *>(this);

    if (wImpl) {

        // get the view of the containing window or scrollview

        WidgetImpl *top = wImpl->GetRootWidget();

        if (top) {
            CocoaWidgetImpl *winImpl = dynamic_cast<CocoaWidgetImpl *>(top);
            if (winImpl) {
                nsView =  winImpl->GetView();
                if (nsView) {
                    NSRect frame;
                    frame = [nsView frame];
                    newY = static_cast<int>(frame.size.height) - y;
                    newY -= height;
                }
            }
        }
    }
    return newY;
}
