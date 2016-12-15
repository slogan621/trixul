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
#include "cocoatextimpl.h"

CocoaTextImpl::CocoaTextImpl()
{
	m_text = NULL;
    m_action = NULL;
}

CocoaTextImpl::~CocoaTextImpl()
{
	if (m_text) 
        [m_text release];
    if (m_action)
        [m_action release];
}

PRStatus CocoaTextImpl::Create()
{
    // XXX Need a huge height or setting a height attribute on the text element
    // will cause positioning to go wacko. If there is no text attribute, then
    // setting to 1.0 will get us a text field that has the right size for the
    // font used (if we use the huge height and there is no height attribute,
    // then the text field will take on that huge height. There should be a 
    // more elegant way to do this. See bug 1734672

    NSRect graphicsRect;

	TextImpl *impl = dynamic_cast<TextImpl *>(this); 

    bool hasHeightAttr = false;

    if (impl)
        hasHeightAttr = impl->GetHasHeightAttr(); 

    if (hasHeightAttr) {
        graphicsRect = NSMakeRect(1.0, 1.0, 1.0, 4096.0);  
    }
    else {
        graphicsRect = NSMakeRect(1.0, 1.0, 1.0, 1.0);  
    }

    m_action = [PasteboardAction alloc];

    [m_action setHandler: this];

	// create the text

	m_text = [[NSTextView alloc] initWithFrame:graphicsRect];

	if (m_text) {
        NSMutableParagraphStyle *style = [[NSParagraphStyle defaultParagraphStyle] mutableCopy]; 
        [style setLineBreakMode: NSLineBreakByTruncatingHead]; 
        [m_text setDefaultParagraphStyle: style];
        [[m_text textContainer] setWidthTracksTextView: YES];
/*
        [m_text setHorizontallyResizable:NO];
        [m_text setAutoresizingMask:NSViewWidthSizable];
        [[m_text textContainer]
            setContainerSize:NSMakeSize(50, FLT_MAX)];
        [[m_text textContainer] setWidthTracksTextView:YES];
*/
        [m_text setDelegate: m_action];
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

PRStatus CocoaTextImpl::SetString(const string& value)
{
	if (m_text) {
		NSString *nsvalue = [NSString stringWithCString: value.c_str()];
		if (nsvalue) {
			[m_text setString: nsvalue];
			return PR_SUCCESS;
		}
	}
	return PR_FAILURE;
}

PRStatus CocoaTextImpl::SetIsEditable(const bool isEditable)
{
	if (m_text) {
        [m_text setEditable: isEditable];
        return PR_SUCCESS;
    }
	return PR_FAILURE;
}

PRStatus CocoaTextImpl::SetIsSelectable(const bool isSelectable)
{
	if (m_text) {
        [m_text setSelectable: isSelectable];
        return PR_SUCCESS;
    }
	return PR_FAILURE;
}

PRStatus CocoaTextImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{
    int newY = AdjustY(y, height);

	if (m_text) {
        NSRect graphicsRect;
        if (mask == GEOM_ALL) {
		    graphicsRect = NSMakeRect(x, newY, width, height);
        }
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
        [[[m_text textContainer] textView] setFrame: graphicsRect];
        [m_text invalidateTextContainerOrigin];
        [[m_text textContainer] setContainerSize: graphicsRect.size];
        [m_text setNeedsDisplay: YES];
        [m_text display];
		return PR_SUCCESS;
	}
	return PR_FAILURE;
}

PRStatus CocoaTextImpl::GetGeometry(int &x, int &y, int &width, int &height)
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
CocoaTextImpl::Show()
{   
    return ShowImpl();
}

PRStatus
CocoaTextImpl::Hide()
{
    return HideImpl();
}       

PRStatus    
CocoaTextImpl::Disable()
{   
    if (m_view) {
        SetIsSelectable(false);
        SetIsEditable(false);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
CocoaTextImpl::Enable()
{
    if (m_view) {
        SetIsSelectable(true);
        SetIsEditable(true);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}       

PRStatus
CocoaTextImpl::GetValue(string &v)
{
    if (m_text) {
        NSString *value = [m_text string];
        if (value) {
            v = [value cStringUsingEncoding: NSUTF8StringEncoding];        
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus
CocoaTextImpl::SetValue(const string &v)
{
    if (m_text) {
        NSString *nsvalue = [NSString stringWithCString: v.c_str()];
        if (nsvalue) 
            [m_text setString: nsvalue];
    }
    return PR_SUCCESS;
    //    return SetString(v);
}

// pasteboardhandler implementation

void CocoaTextImpl::Cut()
{
    NSLog(@"in CocoaTextImpl::Cut");    
    if (m_text) {
        [m_text cut: nil];
    } 
}

void CocoaTextImpl::Copy()
{
    NSLog(@"in CocoaTextImpl::Copy");    
    if (m_text)
        [m_text copy: nil];
}

void CocoaTextImpl::Paste()
{
    NSLog(@"in CocoaTextImpl::Paste");    
    if (m_text)
        [m_text paste: nil];
}

void CocoaTextImpl::Clear()
{
    NSLog(@"in CocoaTextImpl::Clear");    
    if (m_text)
        [m_text delete: nil];
}

void CocoaTextImpl::SelectAll()
{
    NSLog(@"in CocoaTextImpl::SelectAll");    
    if (m_text)
        [m_text selectAll: nil];
}

