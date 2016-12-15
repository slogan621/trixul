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
#include "cocoacheckboximpl.h"

CocoaCheckBoxImpl::CocoaCheckBoxImpl()
{
	m_checkbox = NULL;
}

CocoaCheckBoxImpl::~CocoaCheckBoxImpl()
{
    [m_checkbox release];
}

PRStatus CocoaCheckBoxImpl::Create()
{
    NSRect graphicsRect = NSMakeRect(1.0, 1.0, 1.0, 1.0);

	// create the checkbox

	m_checkbox = [[NSButton alloc] initWithFrame:graphicsRect];

	if (m_checkbox) {
		[m_checkbox setButtonType: NSSwitchButton];
		[m_checkbox setBezelStyle: NSRoundedBezelStyle];
		m_view = (NSView *) m_checkbox;

		WidgetImpl *parentImpl = GetParent();

		if (parentImpl) {

			NSView *parentView = 
                dynamic_cast<CocoaWidgetImpl *>(parentImpl)->GetView();

			// add the checkbox to the parent view

			if (parentView) {
				[parentView addSubview: m_view];
				return PR_SUCCESS;
			}
		}
	}

	return PR_FAILURE;
}

PRStatus CocoaCheckBoxImpl::SetChecked(const bool checked)
{
    PRStatus ret = PR_FAILURE;
    if (m_checkbox) {
        if (checked) {
            [m_checkbox setState: NSOnState];
        } else {
            [m_checkbox setState: NSOffState];
        }
        ret = PR_SUCCESS;
    }
    return ret;     
}

PRStatus CocoaCheckBoxImpl::GetChecked(bool &checked)
{
    PRStatus ret = PR_FAILURE;
    if (m_checkbox) {
        checked = [m_checkbox state];
        ret = PR_SUCCESS;
    }
    return ret;     
}

PRStatus CocoaCheckBoxImpl::SetLabel(const string& label)
{
printf("CocoaCheckBoxImpl::SetLabel this %x label %s\n",
    this, label.c_str());

    if (m_checkbox) {
        NSString *nstitle = [NSString stringWithCString: label.c_str()];
        if (nstitle) {
            [m_checkbox setTitle: nstitle];
printf("SetLabel:  %s ", label.c_str());
            NSFont *font = [NSFont labelFontOfSize:[NSFont labelFontSize]];
            NSRect boundingRect;
            NSSize advancement = [font maximumAdvancement];
printf("Font maxAdv:  %f ", advancement.width);
            boundingRect = [font boundingRectForFont];
printf("boundingRectForFont width:  %f ", boundingRect.size.width);
            float width = 0.0, height;
            width = (int) [nstitle sizeWithAttributes:nil].width;
printf("nsTitle sizeWidthAttr width:  %f ", width);
printf("numChars:  %d\n", [nstitle length]);
            width = 0.0;
            for (int i = 0; i < [nstitle length]; i++) {
printf("glyph:  %d ", i);
                NSSize size;
                NSGlyph glyph;
                glyph = [nstitle characterAtIndex: i];
                boundingRect = [font boundingRectForGlyph: glyph];
printf("glyphBoundingRect width:  %f ", boundingRect.size.width);
                width += boundingRect.size.width;
                advancement = [font advancementForGlyph: glyph];
printf("glyph Adv:  %f\n", advancement.width);
                width += advancement.width;
            }
            boundingRect = [font boundingRectForFont];
            //width += boundingRect.size.width;
            width += 20; // XXX guess
            //height = boundingRect.size.height;
            height = boundingRect.size.height + 10; // XXX guess
            //height = [font ascender] + [font descender] + [font leading];
            SetGeometry(0, 0, width, height, GEOM_WIDTH | GEOM_HEIGHT);
printf("CocoaCheckBoxImpl::SetLabel %s geometry is width %d height %d\n",
    label.c_str(), width, height);
			return PR_SUCCESS;
		}
	}
	return PR_FAILURE;
}

PRStatus 
CocoaCheckBoxImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_checkbox) {
        NSRect graphicsRect;

        graphicsRect = [m_checkbox frame];
        x = static_cast<int>(graphicsRect.origin.x);
        y = static_cast<int>(graphicsRect.origin.y);
        width = static_cast<int>(graphicsRect.size.width);
        height = static_cast<int>(graphicsRect.size.height);
        return PR_SUCCESS;
    }

    return PR_FAILURE;
}

PRStatus 
CocoaCheckBoxImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
printf("CocoaCheckBoxImpl::SetGeometry enter: this %x x %d y %d width %d height %d\n",
    this, x, y, width, height);

    int newY = AdjustY(y, height);
    //int newY = y;

printf("CocoaCheckBoxImpl::SetGeometry after adjust: this %x x %d y %d width %d height %d\n",
    this, x, newY, width, height);

    if (m_checkbox) {
        NSRect graphicsRect;
        if (mask == GEOM_ALL)
            graphicsRect = NSMakeRect(x, newY, width, height);
        else {
            graphicsRect = [m_checkbox frame];
            if (mask & GEOM_X)
                graphicsRect.origin.x = x;
            if (mask & GEOM_Y)
                graphicsRect.origin.y = newY;
            if (mask & GEOM_WIDTH)
                graphicsRect.size.width = width;
            if (mask & GEOM_HEIGHT)
                graphicsRect.size.height = height;
        }

        [m_checkbox setFrame: graphicsRect];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
CocoaCheckBoxImpl::Show()
{
printf("CocoaCheckBoxImpl::Show this %x\n", this);

    return ShowImpl();
}

PRStatus
CocoaCheckBoxImpl::Hide()
{
    return HideImpl();
}

PRStatus
CocoaCheckBoxImpl::Enable()
{
    return EnableImpl();
}

PRStatus
CocoaCheckBoxImpl::Disable()
{
    return DisableImpl();
}
