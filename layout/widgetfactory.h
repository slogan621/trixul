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

#if !defined(__WIDGETFACTORY_H__)
#define __WIDGETFACTORY_H__

// abstract class for widget creation factory. For each widget 
// we can create, there is a MakeFoo function that returns a 
// FooImpl. In each platform widget directory, there is a class 
// that inherits from WidgetFactory. Its MakeFoo function 
// instantiates a concrete class, e.g., GtkFooImpl, that inherits
// from FooImpl, and represents Foo on that platform. 

// The layout Foo abstract class has a reference to an object that 
// belongs to the class FooImpl, which it creates in its constructor. 
// Calls to the Foo class are delegated to that object. Likewise, 
// the Foo class will be a listener to events that are defined in 
// FooImpl and are implemented by the concrete class that inherits 
// from FooImpl.

#include "textimpl.h"
#include "statictextimpl.h"
#include "buttonimpl.h"
#include "windowimpl.h"
#include "boximpl.h"
#include "spacerimpl.h"
#include "appimpl.h"
#include "menuimpl.h"
#include "menubarimpl.h"
#include "menuitemimpl.h"
#include "openpickerimpl.h"
#include "saveaspickerimpl.h"
#include "scrolledwindowimpl.h"
#include "scrolledview.h"
#include "gridlist.h"
#include "checkbox.h"
#include "radiobutton.h"
#include "prtypes.h"

class WidgetFactory {
public:
    virtual ~WidgetFactory() {};
	virtual AppImpl* MakeApp() = 0;
	virtual ButtonImpl* MakeButton() = 0;
	virtual GridListImpl* MakeGridList() = 0;
	virtual StaticTextImpl* MakeStaticText() = 0;
	virtual TextImpl* MakeText() = 0;
	virtual WindowImpl* MakeWindow() = 0;
	virtual BoxImpl* MakeBox() = 0;
	virtual SpacerImpl* MakeSpacer() = 0;
	virtual MenuImpl* MakeMenu() = 0;
	virtual MenubarImpl* MakeMenubar() = 0;
	virtual MenuItemImpl* MakeMenuItem() = 0;
	virtual OpenPickerImpl* MakeOpenPicker() = 0;
	virtual SaveAsPickerImpl* MakeSaveAsPicker() = 0;
	virtual ScrolledWindowImpl* MakeScrolledWindow() = 0;
	virtual ScrolledViewImpl* MakeScrolledView() = 0;
	virtual RadioButtonImpl* MakeRadioButton() = 0;
	virtual CheckBoxImpl* MakeCheckBox() = 0;
};

WidgetFactory *GetWidgetFactory();

#endif
