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

#if !defined(__WINDOW_H__)
#define __WINDOW_H__

#include "container.h"
#include "windowimpl.h"
#include "widgetfactory.h"

#include "windowresizeobserver.h"
#include "windowresizesubject.h"
#include "windowraiseobserver.h"
#include "windowraisesubject.h"

#include "boxlayout.h"

#include <string>
#include <map>

using namespace std;

class Window : public Container, public WindowResizeObserver, public WindowRaiseObserver
{
public:
	Window();
	Window(const string& title);
	virtual ~Window();	
	PRStatus SetTitle(const string& title);
	string& GetTitle();
	PRStatus Draw();
	PRStatus Create();
	PRStatus Show();
	PRStatus Hide();
	PRStatus Close();
    PRStatus ComputeLayout(const int &x, const int &y, const int &width,
        const int &height);
    PRStatus SetGeometry(const int &x, const int &y, 
        const int &width, const int &height, const char &mask);
    PRStatus GetGeometry(int &x, int &y, int &width, int &height);
    PRStatus GetGeometryRequestImpl(int &x, int &y, int &width, int &height) {return PR_SUCCESS;};
    ElementType GetType() {return TYPE_WINDOW;};
    virtual char *GetTypeName() {return("Window");};
    WidgetImpl *GetImpl() {return m_window;}; 
    void WindowResized(const int &x, const int &y, 
        const int &width, const int &height);
    void WindowRaised();
    PRStatus RegisterListener();
    PRStatus UnRegisterListener();
    PRStatus CreateJSObject();
    JSObject *GetJSObject();
    static Window *FindWindowByObj(JSObject *obj);
    void OnLoad();
    bool OnClose();
private:
	WindowImpl *m_window;
	map <string, string> m_entities;	
	PRStatus ParseAndLoadLibraries();
    static map <JSObject *, Window *> m_winObjectMap;
    JSObject *m_jsObj;
};

#endif
