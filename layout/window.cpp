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

#include "window.h"
#include "document.h"
#include <boost/tokenizer.hpp>
#include "spacer.h"
#include "box.h"

Window::Window() : m_jsObj(NULL)
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_window = factory->MakeWindow();
    RegisterListener();
    SetOrientation(Vertical);
}

Window::Window(const string& title)
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_window = factory->MakeWindow();
	SetTitle(title);
    RegisterListener();
}

Window::~Window()
{
	m_entities.clear();
    UnRegisterListener(); 
    if (m_window)
        delete m_window;
}

PRStatus Window::SetTitle(const string& title)
{
	AddAttribute(string("title"), title);

	if (m_window)
		m_window->SetTitle(title);	
	return PR_SUCCESS;
}

PRStatus Window::Close()
{
    if (m_window)
        m_window->Close();	
	return PR_SUCCESS;
}

string& Window::GetTitle()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("title")); 
	if ( attribute ) 
		return attribute->GetValue();
	return failret;
}

PRStatus Window::Draw()
{
	return PR_SUCCESS;
}

PRStatus Window::Create()
{
	PRStatus status;

	status = m_window->Create();
	if (status == PR_SUCCESS) {
		status = SetTitle(GetTitle());	
	}
	if (status == PR_SUCCESS) {
		CreateChildren();
	}
		
	return status;
}

PRStatus Window::Show()
{
	PRStatus status;

	status = m_window->Show();
	if (status == PR_SUCCESS)
		status = ShowChildren();
    
	return status;
}

PRStatus Window::Hide()
{
    return HideChildren();
}

// similar to Box::ComputeLayout, but with a vertical orientation only

PRStatus Window::ComputeLayout(const int &x, const int &y, const int &width,
    const int &height)
{
    int winx, winy, winwidth, winheight;
    int newx, newy, newwidth, newheight;
    char mask;

    newx = x; newy = y; newwidth = width; newheight = height;
    bool hasExplicitGeom = GetExplicitGeometry(mask, winx, winy, 
        winwidth, winheight);

    if (hasExplicitGeom && !GetOverrideExplicitGeometry()) {
        if (mask & GEOM_WIDTH)
            newwidth = winwidth;
        if (mask & GEOM_HEIGHT)
            newheight = winheight;
        if (mask & GEOM_X)
            newx = winx;
        if (mask & GEOM_Y)
            newy = winy;
    }
    return ComputeBoxLayout(newx, newy, newwidth, newheight);
}

PRStatus Window::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_window)
        m_window->GetGeometry(x, y, width, height);

    return PR_SUCCESS;
}

PRStatus Window::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char &mask)
{
    if (m_window)
        m_window->SetGeometry(x, y, width, height, mask);

    return PR_SUCCESS;
}

PRStatus
Window::RegisterListener()
{
    WindowResizeSubject *pWRS =
        dynamic_cast <WindowResizeSubject *>(m_window);
    pWRS->Attach(this);

    WindowRaiseSubject *pWRaiseS =
        dynamic_cast <WindowRaiseSubject *>(m_window);
    pWRaiseS->Attach(this);

    return PR_SUCCESS;
}

PRStatus
Window::UnRegisterListener()
{
    WindowResizeSubject *pWRS =
        dynamic_cast <WindowResizeSubject *>(m_window);
    pWRS->Detach(this);

    WindowRaiseSubject *pWRaiseS =
        dynamic_cast <WindowRaiseSubject *>(m_window);
    pWRaiseS->Detach(this);
    return PR_SUCCESS;
}

void Window::WindowRaised()
{
}

void Window::WindowResized(const int &x, const int &y, 
    const int &width, const int &height)
{
    SetOverrideExplicitGeometry(true);
    ComputeLayout(x, y, width, height);
}

JS_STATIC_DLL_CALLBACK(JSBool)
DoClose(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    // find object to Window * mapping

    JSBool ret = JS_FALSE;
    *rval = 0;
    

    Window *window = Window::FindWindowByObj(obj);
    if (window) {
        window->Close();
    }
    return ret;
}

static JSFunctionSpec window_functions[] = {
    {0, 0, 0, 0, 0}
};

// XXX memory management on this object

PRStatus
Window::CreateJSObject()
{
    JSEngine *js = JSEngine::GetJSEngine();

    if (!js)
        return PR_FAILURE;

    if (m_jsObj)
        return PR_SUCCESS;

    JSContext *ctx = js->GetGlobalContext();
    JSObject *obj = js->GetGlobalObject();

    static JSClass window_class = {
        "window", 0,
        JS_PropertyStub,  JS_PropertyStub,
        JS_PropertyStub,  JS_PropertyStub,
        JS_EnumerateStub,  JS_ResolveStub,
        JS_ConvertStub,   JS_FinalizeStub
    };

    m_jsObj = JS_DefineObject(ctx, obj, "window", &window_class, NULL, 0);

    if (m_jsObj) {
        JS_DefineFunctions(ctx, m_jsObj, window_functions);
        m_winObjectMap.insert(pair<JSObject *, Window *>(m_jsObj, this));
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

JSObject *
Window::GetJSObject()
{
    return m_jsObj;
}

Window *Window::FindWindowByObj(JSObject *obj)
{
    map<JSObject *, Window *>::iterator p;

    p = m_winObjectMap.find(obj);
    if (p != m_winObjectMap.end()) {
        return p->second;
    }
    return NULL;
}       

/**
    This method calls the onload js function
*/

void
Window::OnLoad() 
{
    AnAttribute *attribute;
    Document *doc;
    JSObject *obj;
    JSContext *cx;
    string val;
    attribute = GetAttributeByName(string("onload")); 
   
    doc = GetDocument(); 
    if (doc) {
        obj = doc->GetJSObject();
        cx = doc->GetJSCtx(); 
    }
    
	if (attribute)  {
		val = attribute->GetValue();
        if (val.size()) {
            JSAutoRequest r(cx);
            JSFunction *func = JS_CompileFunction(cx, obj, "onload",
                0, NULL, val.c_str(), val.size(),
                "internal", 1);
            if (func) {
                jsval rval;
                JS_CallFunction(cx, obj, func, 0, NULL, &rval);
            }
        }
    }
    return;
}

/**
    It invokes onunload JS function for the window tag
*/
bool Window::OnClose() 
{
    AnAttribute *attribute;
    Document *doc;
    JSObject *obj;
    JSContext *cx;
    string val;
    bool returnValue = false;
    
	attribute = GetAttributeByName(string("onunload")); 
   
    doc = GetDocument();
    if (doc) {
        obj = doc->GetJSObject();
        cx = doc->GetJSCtx();
    }
 
	if (attribute)  {
		val = attribute->GetValue();
        if (val.size()) {
            JSAutoRequest r(cx);
            JSFunction *func = JS_CompileFunction(cx, obj, "onunload",
                0, NULL, val.c_str(), val.size(),
                "internal", 1);
            if (func) {
                jsval rval;
                JS_CallFunction(cx, obj, func, 0, NULL, &rval);
            }
        }
    }
    return returnValue;
}

map <JSObject *, Window *> Window::m_winObjectMap;

