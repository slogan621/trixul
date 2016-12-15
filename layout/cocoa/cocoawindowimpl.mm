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

#include "../radiogroupmanager.h"

CocoaWindowImpl::CocoaWindowImpl() : m_window(NULL), m_action(NULL)
{
}

CocoaWindowImpl::~CocoaWindowImpl()
{
    RadioGroupManager::RemoveManager(GetDocument());
    if (m_window)
        [m_window release];
    if (m_action)
        [m_action release];
}

PRStatus CocoaWindowImpl::HandleRaise()
{
    NotifyWindowRaise();
    return PR_SUCCESS;
}

PRStatus CocoaWindowImpl::HandleResize(const int &x, const int &y, 
    const int &width, const int &height)
{
    // force a redraw 

    [m_view setNeedsDisplayInRect: [m_view bounds]];
    // notify the document so it can recompute layout

    NotifyWindowResize(x, y, width, height);
    return PR_SUCCESS;
}

PRStatus CocoaWindowImpl::Create()
{
	NSRect graphicsRect;

    RadioGroupManager::AddManager(GetDocument());

    m_action = [ResizeAction alloc];

    [m_action setHandler: this];

	graphicsRect = NSMakeRect(100.0, 350.0, 400.0, 400.0);

	// create the window

	m_window = [[NSWindow alloc]
				initWithContentRect: graphicsRect
				styleMask:NSTitledWindowMask
						|NSClosableWindowMask
						|NSMiniaturizableWindowMask
                        |NSResizableWindowMask
				backing:NSBackingStoreBuffered
				defer:NO ];	

	if (m_window) {
        [m_window retain];
        [m_window setDelegate: m_action];
        //[m_window setAction:@selector(windowDidResize:)];
		// create a view for the window

		m_view = [[[NSView alloc] initWithFrame:graphicsRect]
			autorelease];

		if (m_view) {

			// set the window view

			[m_window setContentView: m_view];
            [m_window setPreservesContentDuringLiveResize: NO];
			return PR_SUCCESS;
		}
	}
	return PR_FAILURE;
}

PRStatus CocoaWindowImpl::Show()
{
    PRStatus ret = PR_FAILURE;

	// show the window

    if (m_window) {
		ret = ShowImpl();
        [m_window makeKeyAndOrderFront: nil];
	}
	return ret;
}

PRStatus CocoaWindowImpl::Hide()
{
    if (m_window) {
        return HideImpl();
    }
    return PR_FAILURE;
}

PRStatus CocoaWindowImpl::Disable()
{
    return PR_SUCCESS;
}

PRStatus CocoaWindowImpl::Enable()
{
    return PR_SUCCESS;
}

PRStatus CocoaWindowImpl::Close()
{
    if (m_window) {
        [m_window close];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus CocoaWindowImpl::SetTitle(const string& title)
{
    // show the window

    if (m_window) {
        NSString *nstitle = [NSString stringWithCString: title.c_str()];
        if (nstitle) {
            [m_window setTitle: nstitle];
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus CocoaWindowImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{
    if (m_window) {
        NSRect graphicsRect;
        if (mask == GEOM_ALL)
            graphicsRect = NSMakeRect(x, y, width, height);
        else {
            graphicsRect = [m_window frame];
            if (mask & GEOM_X)
                graphicsRect.origin.x = x;
            if (mask & GEOM_Y)
                graphicsRect.origin.y = y;
            if (mask & GEOM_WIDTH)
                graphicsRect.size.width = width;
            if (mask & GEOM_HEIGHT)
                graphicsRect.size.height = height;
        }

        [m_window setFrame: graphicsRect display: NO];
        [m_window setContentSize: graphicsRect.size];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus CocoaWindowImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_window) {
        NSRect graphicsRect;
        NSRect contentRect;

        graphicsRect = [m_window frame];
        contentRect = [m_window contentRectForFrameRect: graphicsRect];
        x = static_cast<int>(contentRect.origin.x);
        y = static_cast<int>(contentRect.origin.y);
        width = static_cast<int>(contentRect.size.width);
        height = static_cast<int>(contentRect.size.height);

        return PR_SUCCESS;
    }

    return PR_FAILURE;
}

