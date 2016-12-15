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

#if !defined(__WIDGETIMPL_H__)
#define __WIDGETIMPL_H__

// abstract class for widget implementations

#include "prtypes.h"
#include "element.h"
#include "geometry.h"
#include "document.h"

class WidgetImpl 
{
public:
	WidgetImpl() : m_abstractwidget(NULL), m_parent(NULL), m_document(NULL) {};
	virtual ~WidgetImpl() {};
	virtual PRStatus Create() = 0;
	virtual PRStatus Show() = 0;
	virtual PRStatus Hide() = 0;
    virtual PRStatus Disable() = 0;
    virtual PRStatus Enable() = 0;
    virtual PRStatus SetGeometry(const int &x, const int &y, 
        const int &width, const int &height, const char &mask) = 0;
    virtual PRStatus GetGeometry(int &x, int &y, int &width, int &height) = 0;
    virtual PRStatus SetParent(WidgetImpl *parent) 
    {
        m_parent = parent; 
        return PR_SUCCESS;
    };
    virtual WidgetImpl *GetParent() {return m_parent;};
    virtual WidgetImpl *GetRootWidget();
    void SetType(ElementType type) {m_type = type;};
    ElementType GetType() {return m_type;};
    void SetDocument(Document *document) {m_document = document;};
    Document *GetDocument() {return m_document;};
    void SetWidget(Widget *widget) {m_abstractwidget = widget;};
    Widget *GetWidget() {return m_abstractwidget;};
    // clipboard functions, override if derived class can (or needs to) 
    // handle. (Cocoa is an example of a platform that doesn't need to
    // for its NSTextField class.)
    virtual void OnCut() {};
    virtual void OnCopy() {};
    virtual void OnPaste() {};
    virtual void OnClear() {};
    virtual void OnSelectAll() {};
private:
    Widget *m_abstractwidget; 
	WidgetImpl *m_parent;
    ElementType m_type;
    Document *m_document; 
};

#endif
