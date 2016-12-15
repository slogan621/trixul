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

[NSFont systemFontOfSize:[NSFont systemFontSizeForControlSize: NSRegularControlSize]]

*/

#include "cocoawindowimpl.h"
#include "cocoastatictextimpl.h"

CocoaStaticTextImpl::CocoaStaticTextImpl()
{
	m_text = NULL;
}

CocoaStaticTextImpl::~CocoaStaticTextImpl()
{
    if (m_text)
        [m_text release];
}

PRStatus CocoaStaticTextImpl::Create()
{
    NSRect graphicsRect = NSMakeRect(1.0, 1.0, 1.0, 1.0);

	// create the text

	m_text = [[NSTextField alloc] initWithFrame:graphicsRect];

	if (m_text) {
		m_view = (NSView *) m_text;

		WidgetImpl *parentImpl = GetParent();

		if (parentImpl) {

			NSView *parentView = dynamic_cast<CocoaWidgetImpl *>(parentImpl)->GetView();

			// add the text to the parent view

			if (parentView) {
				[parentView addSubview: m_view];
				return PR_SUCCESS;
			}
		}
	}

	return PR_FAILURE;
}

PRStatus CocoaStaticTextImpl::SetString(const string& value)
{
    if (m_text) {
        NSString *nsvalue = [NSString stringWithCString: value.c_str()];
        if (nsvalue) {
            [m_text setStringValue: nsvalue];
            [m_text setDrawsBackground: NO];
            [m_text setEditable: NO];
            [m_text setBordered: NO];
            [m_text sizeToFit];
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus CocoaStaticTextImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{
    int newY = AdjustY(y, height);

    if (m_text) {
        NSRect graphicsRect;
        if (mask == GEOM_ALL)
            graphicsRect = NSMakeRect(x, newY, width, height);
        else {
            graphicsRect = [m_text frame];
            if (mask & GEOM_X)
                graphicsRect.origin.x = x;
            if (mask & GEOM_Y)
                graphicsRect.origin.y = newY;
            if (mask & GEOM_WIDTH)
                graphicsRect.size.width = width;
            if (mask & GEOM_HEIGHT)
                graphicsRect.size.height = height;
        }

        [m_text setFrame: graphicsRect];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus 
CocoaStaticTextImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_text) {
        NSRect graphicsRect;

        graphicsRect = [m_text frame];
        x = static_cast<int>(graphicsRect.origin.x);
        y = static_cast<int>(graphicsRect.origin.y);
        width = static_cast<int>(graphicsRect.size.width);
        height = static_cast<int>(graphicsRect.size.height);

        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus    
CocoaStaticTextImpl::Show()
{   
    return ShowImpl();
}

PRStatus
CocoaStaticTextImpl::Hide()
{
    return HideImpl();
}       

PRStatus    
CocoaStaticTextImpl::Enable()
{   
    return EnableImpl();
}

PRStatus
CocoaStaticTextImpl::Disable()
{
    return DisableImpl();
}       


