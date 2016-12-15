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

#if !defined( __ELEMENT_H__ )
#define __ELEMENT_H__

#include <list>
#include <string>
#include <map>
#include "attributelist.h"

#include "prtypes.h"

#include "jsengine.h"
#include "abstractelementvisitor.h"

typedef enum {
	TYPE_ROOT,		// top node in document hierarchy
	TYPE_WINDOW,
	TYPE_TEXT,
	TYPE_STATIC_TEXT,
	TYPE_BUTTON,
	TYPE_GRIDLIST,
	TYPE_MENU,
	TYPE_MENUBAR,
	TYPE_MENUITEM,
	TYPE_IMAGE,
	TYPE_SPACER,
	TYPE_BOX,
	TYPE_RADIOBUTTON,
	TYPE_CHECKBOX,
	TYPE_SCROLLEDWINDOW,
	TYPE_SCROLLEDVIEW
} ElementType;

class Document;
class Control;

class Element : public AttributeList {
public:
	Element();
	Element(Element *parent);
	virtual ~Element() {};
    virtual void PrintInformation();
	void SetType(const ElementType type);
	ElementType GetType();
	void SetLevel(const int level);
	int GetLevel();
	Document *GetDocument();
	void SetDocument(Document *doc);
	Element *GetParent(); 
	void SetParent(Element *parent); 
	void AddChild(Element *element, const int level);
    PRStatus SetValueHelper(JSContext *cx, Control *control, jsval val, 
        jsval *rval);
    PRStatus GetValueHelper(JSContext *cx, Control *control, jsval *rval);
    PRStatus CreateJSObject();
    JSObject *GetJSObject();
    static Element *FindElementByObj(JSObject *obj);
    list <Element *>& GetChildren() {return m_children;};
	void Accept(AbstractElementVisitor *elementVisitor) ;    
private:
	Element *m_parent;
	int m_level;
	ElementType m_type;
    Document *m_doc;
    JSObject *m_jsObj;    
    static map <JSObject *, Element *> m_objectMap;
	list <Element *> m_children;   
};

#endif
