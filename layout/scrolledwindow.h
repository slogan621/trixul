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

#if !defined(__SCROLLEDWINDOW_H__)
#define __SCROLLEDWINDOW_H__

#include "container.h"
#include "scrolledwindowimpl.h"
#include "widgetfactory.h"

class ScrolledWindow : public Container
{
public:
	ScrolledWindow();
    ~ScrolledWindow();
	PRStatus Create();
	PRStatus Draw();
	PRStatus Show();
	PRStatus Hide();
    PRStatus GetGeometry(int &x, int &y, int &width, int &height);
    PRStatus SetGeometry(const int &x, const int &y, 
        const int &width, const int &height, const char &mask);
    PRStatus GetGeometryRequestImpl(int &x, int &y, int &width, int &height) {return PR_SUCCESS;};
	ElementType GetType() {return TYPE_SCROLLEDWINDOW;};
    virtual char *GetTypeName() {return("ScrolledWindow");};
	WidgetImpl *GetImpl() {return m_scrolledWindow;}; 
    PRStatus ComputeLayout(const int &x, const int &y, 
    const int &width, const int &height);
private:
	ScrolledWindowImpl *m_scrolledWindow;
};

#endif
