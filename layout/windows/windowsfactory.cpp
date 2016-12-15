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

#include "windowsfactory.h"

#include "windowstextimpl.h"
#include "windowsstatictextimpl.h"
#include "windowsbuttonimpl.h"
#include "windowswindowimpl.h"
#include "windowsmenuimpl.h"
#include "windowsmenubarimpl.h"
#include "windowsmenuitemimpl.h"
#include "windowsopenpickerimpl.h"
#include "windowssaveaspickerimpl.h"
#include "windowsscrolledwindowimpl.h"
#include "windowsscrolledviewimpl.h"
#include "windowsgridlistimpl.h"
#include "windowscheckboximpl.h"
#include "windowsradiobuttonimpl.h"
#include "windowsappimpl.h"

WindowsFactory::WindowsFactory() : m_appImpl(NULL)
{
}

WindowsFactory::~WindowsFactory()
{
}

// create a ButtonImpl widget

ButtonImpl *WindowsFactory::MakeButton()
{
	return new WindowsButtonImpl;
}

// create a RadioButtonImpl widget

RadioButtonImpl *WindowsFactory::MakeRadioButton()
{
	return new WindowsRadioButtonImpl;
}

// create a CheckBoxImpl widget

CheckBoxImpl *WindowsFactory::MakeCheckBox()
{
	return new WindowsCheckBoxImpl;
}

// create a TextImpl widget

TextImpl *WindowsFactory::MakeText()
{
	return new WindowsTextImpl;
}

// create a GridListImpl widget

GridListImpl *WindowsFactory::MakeGridList()
{
	return new WindowsGridListImpl;
}

// create a StaticTextImpl widget

StaticTextImpl *WindowsFactory::MakeStaticText()
{
	return new WindowsStaticTextImpl;
}

// create a WindowImpl widget

WindowImpl *WindowsFactory::MakeWindow()
{
	return new WindowsWindowImpl;
}

OpenPickerImpl *WindowsFactory::MakeOpenPicker()
{
	return new WindowsOpenPickerImpl;
}

SaveAsPickerImpl *WindowsFactory::MakeSaveAsPicker()
{
	return new WindowsSaveAsPickerImpl;
}

// create a MenuImpl widget

MenuImpl *WindowsFactory::MakeMenu()
{
	return new WindowsMenuImpl;
}

// create a MenubarImpl widget

MenubarImpl *WindowsFactory::MakeMenubar()
{
	return new WindowsMenubarImpl;
}

// create a MenuItemImpl widget

MenuItemImpl *WindowsFactory::MakeMenuItem()
{
	return new WindowsMenuItemImpl;
}

// create a ScrolledView widget

ScrolledViewImpl *WindowsFactory::MakeScrolledView()
{
	return new WindowsScrolledViewImpl;
}

// create a ScrolledWindow widget

ScrolledWindowImpl *WindowsFactory::MakeScrolledWindow()
{
	return new WindowsScrolledWindowImpl;
}

// create a BoxImpl widget

BoxImpl *WindowsFactory::MakeBox()
{
	return NULL;
}

// create a SpacerImpl widget

SpacerImpl *WindowsFactory::MakeSpacer()
{
	return NULL;
}

// create an AppImpl object

AppImpl *WindowsFactory::MakeApp()
{
    if (!m_appImpl)
        m_appImpl = WindowsAppImpl::GetWindowsAppImplInstance();
	return m_appImpl;
}
