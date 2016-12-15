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

#include "gtkfactory.h"

#include "gtktextimpl.h"
#include "gtkstatictextimpl.h"
#include "gtkbuttonimpl.h"
#include "gtkwindowimpl.h"
#include "gtkmenuimpl.h"
#include "gtkmenuitemimpl.h"
#include "gtkmenubarimpl.h"
#include "gtksaveaspickerimpl.h"
#include "gtkopenpickerimpl.h"
#include "gtkscrolledviewimpl.h"
#include "gtkscrolledwindowimpl.h"
#include "gtkgridlistimpl.h"
#include "gtkcheckboximpl.h"
#include "gtkradiobuttonimpl.h"
#include "gtkappimpl.h"

GtkFactory::GtkFactory() : m_appImpl(NULL)
{
}

GtkFactory::~GtkFactory()
{
}

// create a ButtonImpl widget

ButtonImpl *GtkFactory::MakeButton()
{
	return new GtkButtonImpl;
}

OpenPickerImpl *GtkFactory::MakeOpenPicker()
{
	return new GtkOpenPickerImpl;
}

SaveAsPickerImpl *GtkFactory::MakeSaveAsPicker()
{
	return new GtkSaveAsPickerImpl;
}

// create a TextImpl widget

TextImpl *GtkFactory::MakeText()
{
	return new GtkTextImpl;
}

// create a StaticTextImpl widget

StaticTextImpl *GtkFactory::MakeStaticText()
{
	return new GtkStaticTextImpl;
}

// create a GridListImpl widget

GridListImpl *GtkFactory::MakeGridList()
{
	return new GtkGridListImpl;
}

// create a CheckBoxImpl widget

CheckBoxImpl *GtkFactory::MakeCheckBox()
{
	return new GtkCheckBoxImpl;
}

// create a RadioButtonImpl widget

RadioButtonImpl *GtkFactory::MakeRadioButton()
{
	return new GtkRadioButtonImpl;
}

MenuImpl *GtkFactory::MakeMenu()
{
	return new GtkMenuImpl;
}

MenubarImpl *GtkFactory::MakeMenubar()
{
	return new GtkMenubarImpl;
}

MenuItemImpl *GtkFactory::MakeMenuItem()
{
	return new GtkMenuItemImpl;
}

// create a WindowImpl widget

WindowImpl *GtkFactory::MakeWindow()
{
	return new GtkWindowImpl;
}

// create a BoxImpl widget

BoxImpl *GtkFactory::MakeBox()
{
	return NULL;
}

ScrolledWindowImpl *GtkFactory::MakeScrolledWindow()
{
	return new GtkScrolledWindowImpl;
}

ScrolledViewImpl *GtkFactory::MakeScrolledView()
{
	return new GtkScrolledViewImpl;
}

// create a SpacerImpl widget

SpacerImpl *GtkFactory::MakeSpacer()
{
	return NULL;
}

// create an AppImpl object

AppImpl *GtkFactory::MakeApp()
{
    if (!m_appImpl)
        m_appImpl = GtkAppImpl::GetGtkAppImplInstance();
	return m_appImpl;
}
