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

#if !defined(__COCOA_FACTORY_H__)
#define __COCOA_FACTORY_H__

#include "../widgetfactory.h"
#include "../textimpl.h"
#include "../statictextimpl.h"
#include "../buttonimpl.h"
#include "../menuimpl.h"
#include "../menubarimpl.h"
#include "../menuitemimpl.h"
#include "../boximpl.h"
#include "../spacerimpl.h"
#include "../windowimpl.h"
#include "../windowimpl.h"
#include "../openpickerimpl.h"
#include "../saveaspickerimpl.h"
#include "../scrolledwindow.h"
#include "../scrolledview.h"
#include "../gridlist.h"
#include "../radiobutton.h"
#include "../checkbox.h"
#include "prtypes.h"

class CocoaFactory : public WidgetFactory
{
public:
    static CocoaFactory *GetFactoryInstance()
    {
        static CocoaFactory *factory = NULL;
        if (!factory)
            factory = new CocoaFactory;
        return factory;
    }
	virtual ~CocoaFactory();
	virtual TextImpl *MakeText(); 
	virtual StaticTextImpl *MakeStaticText(); 
	virtual ButtonImpl *MakeButton(); 
	virtual MenuImpl *MakeMenu(); 
	virtual MenubarImpl *MakeMenubar(); 
	virtual MenuItemImpl *MakeMenuItem(); 
	virtual BoxImpl *MakeBox(); 
	virtual SpacerImpl *MakeSpacer(); 
	virtual WindowImpl *MakeWindow(); 
	virtual OpenPickerImpl *MakeOpenPicker(); 
	virtual SaveAsPickerImpl *MakeSaveAsPicker(); 
	virtual ScrolledWindowImpl *MakeScrolledWindow(); 
	virtual ScrolledViewImpl *MakeScrolledView(); 
	virtual GridListImpl *MakeGridList(); 
	virtual RadioButtonImpl *MakeRadioButton(); 
	virtual CheckBoxImpl *MakeCheckBox(); 
	virtual AppImpl *MakeApp(); 
    AppImpl *GetAppImpl() {return m_appImpl;};
private:
	CocoaFactory();
    AppImpl *m_appImpl;
};

#endif
