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
#include "cocoaradiobuttonimpl.h"
#include "../radiogroupmanager.h"

CocoaRadioButtonImpl::CocoaRadioButtonImpl()
{
	m_radiobutton = NULL;
    m_action = NULL;
}

CocoaRadioButtonImpl::~CocoaRadioButtonImpl()
{
	// destroy the radiobutton
    RadioGroupManager *mgr = RadioGroupManager::GetManager(GetDocument());
    if (mgr)
        mgr->RemoveFromGroup(m_group, this);
    if (m_radiobutton)
        [m_radiobutton release];
    if (m_action)
        [m_action release];
}

PRStatus CocoaRadioButtonImpl::Create()
{
    NSRect graphicsRect = NSMakeRect(1.0, 1.0, 1.0, 1.0);

    m_action = [ButtonAction alloc];

    [m_action setHandler: this];

	// create the radiobutton

	m_radiobutton = [[NSButton alloc] initWithFrame:graphicsRect];

	if (m_radiobutton) {
		[m_radiobutton setButtonType: NSRadioButton];
		[m_radiobutton setBezelStyle: NSRoundedBezelStyle];
        [m_radiobutton setTarget:m_action];
        [m_radiobutton setAction:@selector(onClick:)];

		m_view = (NSView *) m_radiobutton;

		WidgetImpl *parentImpl = GetParent();

		if (parentImpl) {

			NSView *parentView = 
                dynamic_cast<CocoaWidgetImpl *>(parentImpl)->GetView();

			// add the radiobutton to the parent view

			if (parentView) {
				[parentView addSubview: m_view];
				return PR_SUCCESS;
			}
		}
	}

	return PR_FAILURE;
}

PRStatus CocoaRadioButtonImpl::HandleCommand()
{
    if (m_radiobutton) {
        RadioGroupManager *mgr = RadioGroupManager::GetManager(GetDocument());
        if (mgr)
            mgr->SetState(m_group, this);
    }
    return PR_SUCCESS;
}

PRStatus CocoaRadioButtonImpl::SetChecked(const bool checked)
{           
    PRStatus ret = PR_FAILURE;
    if (m_radiobutton) {
        if (checked) {
            [m_radiobutton setState: NSOnState];
            HandleCommand(); // propogate the change
        } else {
            [m_radiobutton setState: NSOffState];
        }
        ret = PR_SUCCESS;
    }       
    return ret;  
}           

PRStatus CocoaRadioButtonImpl::GetChecked(bool &checked)
{
    PRStatus ret = PR_FAILURE;
    if (m_radiobutton) {
        checked = [m_radiobutton state];
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus CocoaRadioButtonImpl::SetGroup(const string &group)
{
    PRStatus ret = PR_SUCCESS;
    m_group = group;
    RadioGroupManager *mgr = RadioGroupManager::GetManager(GetDocument());
    if (mgr)
        mgr->AddToGroup(m_group, this);
    return ret;
}

PRStatus CocoaRadioButtonImpl::SetState(const bool state)
{
    PRStatus ret = PR_FAILURE;

    int cState = (state == true?NSOnState:NSOffState);
    if (m_radiobutton) {
        [m_radiobutton setState: cState];
        RadioGroupManager *mgr = RadioGroupManager::GetManager(GetDocument());
        if (mgr && state == true) {
            mgr->SetState(m_group, this);
        }
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus CocoaRadioButtonImpl::SetLabel(const string& label)
{
    if (m_radiobutton) {
        NSString *nstitle = [NSString stringWithCString: label.c_str()];
        if (nstitle) {
            [m_radiobutton setTitle: nstitle];
            NSFont *font = [NSFont labelFontOfSize:[NSFont labelFontSize]];
            NSRect boundingRect;
            NSSize advancement = [font maximumAdvancement];
            boundingRect = [font boundingRectForFont];
            float width = 0.0, height;
            width = (int) [nstitle sizeWithAttributes:nil].width;
            width = 0.0;
            for (int i = 0; i < (int) [nstitle length]; i++) {
                NSGlyph glyph;
                glyph = [nstitle characterAtIndex: i];
                boundingRect = [font boundingRectForGlyph: glyph];
                width += boundingRect.size.width;
                advancement = [font advancementForGlyph: glyph];
                width += advancement.width;
            }
            boundingRect = [font boundingRectForFont];
            //width += boundingRect.size.width;
            width += 20; // XXX guess
            //height = boundingRect.size.height;
            height = boundingRect.size.height + 10; // XXX guess
            //height = [font ascender] + [font descender] + [font leading];
            SetGeometry(0, 0, width, height, GEOM_WIDTH | GEOM_HEIGHT);
			return PR_SUCCESS;
		}
	}
	return PR_FAILURE;
}

PRStatus 
CocoaRadioButtonImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_radiobutton) {
        NSRect graphicsRect;

        graphicsRect = [m_radiobutton frame];
        x = static_cast<int>(graphicsRect.origin.x);
        y = static_cast<int>(graphicsRect.origin.y);
        width = static_cast<int>(graphicsRect.size.width);
        height = static_cast<int>(graphicsRect.size.height);
        return PR_SUCCESS;
    }

    return PR_FAILURE;
}

PRStatus 
CocoaRadioButtonImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    int newY = AdjustY(y, height);

    if (m_radiobutton) {
        NSRect graphicsRect;
        if (mask == GEOM_ALL)
            graphicsRect = NSMakeRect(x, newY, width, height);
        else {
            graphicsRect = [m_radiobutton frame];
            if (mask & GEOM_X)
                graphicsRect.origin.x = x;
            if (mask & GEOM_Y)
                graphicsRect.origin.y = newY;
            if (mask & GEOM_WIDTH)
                graphicsRect.size.width = width;
            if (mask & GEOM_HEIGHT)
                graphicsRect.size.height = height;
        }

        [m_radiobutton setFrame: graphicsRect];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
CocoaRadioButtonImpl::Show()
{
    return ShowImpl();
}

PRStatus
CocoaRadioButtonImpl::Hide()
{
    return HideImpl();
}

PRStatus
CocoaRadioButtonImpl::Enable()
{
    return EnableImpl();
}

PRStatus
CocoaRadioButtonImpl::Disable()
{
    return DisableImpl();
}

