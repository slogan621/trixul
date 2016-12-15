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

#include "cocoafactory.h"

#include "cocoatextimpl.h"
#include "cocoastatictextimpl.h"
#include "cocoabuttonimpl.h"
#include "cocoamenuimpl.h"
#include "cocoamenubarimpl.h"
#include "cocoamenuitemimpl.h"
#include "cocoawindowimpl.h"
#include "cocoaopenpickerimpl.h"
#include "cocoasaveaspickerimpl.h"
#include "cocoascrolledwindowimpl.h"
#include "cocoascrolledviewimpl.h"
#include "cocoagridlistimpl.h"
#include "cocoaradiobuttonimpl.h"
#include "cocoacheckboximpl.h"
#include "cocoaappimpl.h"

CocoaFactory::CocoaFactory() : m_appImpl(NULL)
{
}

CocoaFactory::~CocoaFactory()
{
}

// create a ButtonImpl widget

ButtonImpl *CocoaFactory::MakeButton()
{
	return new CocoaButtonImpl;
}

// create a RadioButtonImpl widget

RadioButtonImpl *CocoaFactory::MakeRadioButton()
{
	return new CocoaRadioButtonImpl;
}

// create a CheckBoxImpl widget

CheckBoxImpl *CocoaFactory::MakeCheckBox()
{
	return new CocoaCheckBoxImpl;
}

// create a GridListImpl widget

GridListImpl *CocoaFactory::MakeGridList()
{
	return new CocoaGridListImpl;
}

ScrolledWindowImpl *CocoaFactory::MakeScrolledWindow()
{
	return new CocoaScrolledWindowImpl;
}

ScrolledViewImpl *CocoaFactory::MakeScrolledView()
{
	return new CocoaScrolledViewImpl;
}

OpenPickerImpl *CocoaFactory::MakeOpenPicker()
{
	return new CocoaOpenPickerImpl;
}

SaveAsPickerImpl *CocoaFactory::MakeSaveAsPicker()
{
	return new CocoaSaveAsPickerImpl;
}

// create a MenuImpl widget

MenuImpl *CocoaFactory::MakeMenu()
{
	return new CocoaMenuImpl;
}

// create a MenuItemImpl widget

MenuItemImpl *CocoaFactory::MakeMenuItem()
{
	return new CocoaMenuItemImpl;
}

// create a MenubarImpl widget

MenubarImpl *CocoaFactory::MakeMenubar()
{
	return new CocoaMenubarImpl;
}

// create a TextImpl widget

TextImpl *CocoaFactory::MakeText()
{
	return new CocoaTextImpl;
}

// create a StaticTextImpl widget

StaticTextImpl *CocoaFactory::MakeStaticText()
{
	return new CocoaStaticTextImpl;
}

// create a WindowImpl widget

WindowImpl *CocoaFactory::MakeWindow()
{
	return new CocoaWindowImpl;
}

// create a BoxImpl widget

BoxImpl *CocoaFactory::MakeBox()
{
	return NULL;
}

// create a SpacerImpl widget

SpacerImpl *CocoaFactory::MakeSpacer()
{
	return NULL;
}

// create an AppImpl object

AppImpl *CocoaFactory::MakeApp()
{
    if (!m_appImpl)
        m_appImpl = CocoaAppImpl::GetCocoaAppImplInstance();
    return m_appImpl;
}

