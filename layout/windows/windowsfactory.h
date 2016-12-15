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

#if !defined(__WINDOWS_FACTORY__H__)
#define __WINDOWS_FACTORY__H__

#include "../widgetfactory.h"
#include "../textimpl.h"
#include "../statictextimpl.h"
#include "../buttonimpl.h"
#include "../boximpl.h"
#include "../spacerimpl.h"
#include "../windowimpl.h"
#include "../menuimpl.h"
#include "../menubarimpl.h"
#include "../menuitemimpl.h"
#include "../openpickerimpl.h"
#include "../saveaspickerimpl.h"
#include "../scrolledwindowimpl.h"
#include "../scrolledviewimpl.h"
#include "../gridlistimpl.h"
#include "../checkboximpl.h"
#include "../radiobuttonimpl.h"
#include "../appimpl.h"
#include "prtypes.h"

class WindowsFactory : public WidgetFactory
{
public:
    static WindowsFactory *GetFactoryInstance()
    {
        static WindowsFactory *factory = NULL;
        if (!factory)
            factory = new WindowsFactory;
        return factory;
    }
	virtual ~WindowsFactory();
	virtual TextImpl *MakeText(); 
	virtual StaticTextImpl *MakeStaticText(); 
	virtual ButtonImpl *MakeButton(); 
	virtual BoxImpl *MakeBox(); 
	virtual SpacerImpl *MakeSpacer(); 
	virtual WindowImpl *MakeWindow(); 
	virtual MenuImpl *MakeMenu(); 
	virtual MenubarImpl *MakeMenubar(); 
	virtual MenuItemImpl *MakeMenuItem(); 
	virtual OpenPickerImpl *MakeOpenPicker(); 
	virtual SaveAsPickerImpl *MakeSaveAsPicker(); 
	virtual ScrolledViewImpl *MakeScrolledView(); 
	virtual ScrolledWindowImpl *MakeScrolledWindow(); 
	virtual GridListImpl *MakeGridList(); 
	virtual CheckBoxImpl *MakeCheckBox(); 
	virtual RadioButtonImpl *MakeRadioButton(); 
	virtual AppImpl *MakeApp(); 
private:
	WindowsFactory();
    AppImpl *m_appImpl;
};

#endif
