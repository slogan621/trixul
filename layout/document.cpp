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

#include "document.h"
#include <iostream>

#include "window.h"
#include "layout.h"

#include "plstr.h"

#include "prettyprintvisitor.h"

using namespace std;

//	string path;
//	list <Element *> elements;

Document::Document(const string& path) 
{
	m_path = path;
	m_root = m_current = new Element(NULL);
	m_root->SetType(TYPE_ROOT);
	m_root->SetLevel(0);
    m_scripts.clear();
    m_jsObj = NULL;
    elementVisitor = new PrettyPrintVisitor();
}

Document::~Document()
{
    Element *node = GetWindow(NULL, false);
    if (node) { 
        Window *w = reinterpret_cast<Window *>(node);
        if (w)
            w->OnClose();
    }

    CloseScripts();

    m_scripts.clear();
    delete elementVisitor;

    JSEngine *js = JSEngine::GetJSEngine();

    if (js) 
        js->FreeContext(m_jsCtx);

    list<Element *>::iterator itr;
    list<Element *> children;

    children = m_root->GetChildren();

    for (itr = children.begin(); itr != children.end(); ++itr)
        delete (*itr);

    if (m_root)
        delete m_root;
}

void Document::Dump()
{
	Dump(m_root);
}

// XXX Fix me, this begs for application of visitor pattern

void Document::Dump(Element *root)
{
	if (!root)
		return;
	
    root->Accept(elementVisitor);
    
    
	list<Element *>::iterator itr;
    list<Element *> children;

    children = root->GetChildren();

	for (itr = children.begin(); itr != children.end(); ++itr) 
	{
		Dump(*itr);
	}
}

Element *Document::GetMainWindow(Element *root)
{
    return GetWindow(root, true);
}

Element *Document::GetWindow(Element *root, bool isMain)
{
	// if passed with NULL, start with document root

	if (!root)
		root = m_root;

	// if still NULL, bail

	if (!root)
		return NULL;

	// check if this node is the one

	if (root->GetType() == TYPE_WINDOW) {
		AnAttribute *attr;
		
		if (!isMain || (isMain && (attr = root->GetAttributeByName("main")) 
            && attr->GetValue() == "true"))
		{
			return root;
		}
	}

	// if we get here, this node isn't it -- so check children

	list<Element *>::iterator itr;
	Element *node;
    list<Element *> children;

    children = root->GetChildren();

	for (itr = children.begin(); itr != children.end(); ++itr) {
		if ((node = GetWindow(*itr, isMain)))
			return node;
	}

	return NULL;
}

Element *Document::GetElementById(Element *root, const string& id)
{
	if (!root)
		root = m_root;

	// if still NULL, bail

	if (!root)
		return NULL;

    AnAttribute *attr;

    if (((attr = root->GetAttributeByName("id")) && 
        !PL_strcasecmp(id.c_str(), attr->GetValue().c_str())))
    {
        return root;
    } 

    // if we get here, this node isn't it -- so check children
    
    list<Element *>::iterator itr;
    Element *node;

    list<Element *> children;

    children = root->GetChildren();
    for (itr = children.begin(); itr != children.end(); ++itr) {
        if ((node = GetElementById(*itr, id)))
            return node;
    }
    return NULL;
}

string& Document::GetPath()
{
	return m_path;
}

void Document::SetCurrent(Element *element)
{
	m_current = element;
}

Element *Document::GetCurrent()
{
	return m_current;
}

void Document::SetLevel(int level)
{
	m_level = level;
}

int Document::GetLevel()
{
	return m_level;
}

PRStatus Document::AddScript(Script *script)
{
    // append the script to the list

    m_scripts.push_back(script);
    return PR_SUCCESS;
}

PRStatus Document::ParseScripts()
{
    list <Script *>::iterator itr;

	for (itr = m_scripts.begin(); itr != m_scripts.end(); ++itr) {
        (*itr)->Parse(m_jsObj, m_jsCtx);
	}
    return PR_SUCCESS;
}

PRStatus Document::CloseScripts()
{
    list <Script *>::iterator itr;

	for (itr = m_scripts.begin(); itr != m_scripts.end(); ++itr) {
        (*itr)->Close();
        delete (*itr);
	}
    return PR_SUCCESS;
}

Document *Document::FindDocumentByObj(JSObject *obj)
{
    map<JSObject *, Document *>::iterator p;

    p = m_documentObjectMap.find(obj);
    if (p != m_documentObjectMap.end())
    {
        return p->second;
    }
    return NULL;
}

Window *Document::FindWindowByObj(JSObject *obj)
{
    Document *doc = FindDocumentByObj(obj); 
    if (doc) 
        return (reinterpret_cast<Window *>(doc->GetWindow(NULL, false)));
    return NULL;
}

PRStatus Document::ComputeLayout()
{
    Element *node;

    node = GetWindow(NULL, false);
    if (node) {	
        Window *w = reinterpret_cast<Window *>(node);
        int x, y, width, height; 
        w->GetGeometry(x, y, width, height);
        w->SetGeometryRequest(x, y, width, height, GEOM_ALL); 
        w->ComputeLayout(x, y, width, height);
        
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

// JS support for document

JS_STATIC_DLL_CALLBACK(JSBool)
DoWinClose(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    // find object to Window * mapping

    JSBool ret = JS_FALSE;

    Window *w = Document::FindWindowByObj(obj);
    Document *doc = Document::FindDocumentByObj(obj);

    if (w /*&& w->AllowClose()*/) {
        w->Close();
        Document::RemoveFromObjMap(obj);
        Layout *layout = Layout::GetInstance();
        if (layout) 
            layout->RemoveDocumentFromMap(doc);
    }

    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
DoOpenDialog(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    // find object to Window * mapping

    JSBool ret = JS_FALSE;

    Layout *layout = Layout::GetInstance();

    if (layout) {
        JSString *str;

        str = JS_ValueToString(cx, argv[0]);
        if (!str)
            return JS_FALSE;

        char *bytes = JS_GetStringBytes(str);
        bytes = strdup(bytes);
        if (bytes) {
            Document *doc;
            if ((doc = layout->ProcessFile(bytes)) != (Document *)NULL) {
                doc->Dump();
                doc->ParseScripts();
   
                // find the window element, convert it to a widget, and 
                // then compute its layout and show it

                Window *win;

                win = reinterpret_cast<Window *>(doc->GetWindow(NULL, false));
                if (win) {
                    if (win && win->Create() == PR_SUCCESS) {
                        // compute layouts of all widgets

                        doc->ComputeLayout(); 
                        win->Show();
                        win->OnLoad();
                        ret = JS_TRUE;
                    }
                }
            }
            free(bytes);
        }
    }

    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
DoGetElementById(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    // find object to Document *mapping

    JSBool ret = JS_FALSE;
    *rval = 0;

    Document *document = Document::FindDocumentByObj(obj);
    if (document) {
        JSString *str;

        str = JS_ValueToString(cx, argv[0]);
        if (!str)
            return JS_FALSE;

        char *bytes = JS_GetStringBytes(str);
        bytes = strdup(bytes);
        if (bytes) {
            Element *element = document->GetElementById(NULL, bytes);
            if (element) {
                if (element->CreateJSObject() == PR_SUCCESS) {
                    JSObject *jsobj = element->GetJSObject();
                    if (jsobj) {
                        *rval = OBJECT_TO_JSVAL(jsobj);
                        ret = JS_TRUE;
                    }
                }
            }
            free(bytes);
        }
    } 
    return ret;
}
 
static JSFunctionSpec document_functions[] = {
    {"getElementById", DoGetElementById,  1, 0, 0},
    {"close", DoWinClose,  1, 0, 0},
    {"openDialog", DoOpenDialog,  1, 0, 0},
	JS_FS_END
};

// XXX memory management on this object

PRStatus
Document::CreateJSObject()
{
    JSEngine *js = JSEngine::GetJSEngine();

    if (!js)
        return PR_FAILURE;

    if (m_jsObj)
        return PR_SUCCESS;

    JSContext *ctx = js->GetNewContext(); 
    m_jsCtx = ctx;

    JSObject *obj = js->NewSuperGlobal(ctx);

    static JSClass document_class = {
        "document", 0,
        JS_PropertyStub,  JS_PropertyStub,
        JS_PropertyStub,  JS_PropertyStub,
        JS_EnumerateStub,  JS_ResolveStub,
        JS_ConvertStub,   JS_FinalizeStub
    };

    JSAutoRequest r(m_jsCtx);
    m_jsObj = JS_DefineObject(m_jsCtx, obj, "document", &document_class, obj, 0);

    if (m_jsObj) {
        JS_DefineFunctions(m_jsCtx, m_jsObj, document_functions);
        m_documentObjectMap.insert(pair<JSObject *, Document *>(m_jsObj, this));
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

JSObject *
Document::GetJSObject()
{
    return m_jsObj;
}

JSContext *
Document::GetJSCtx()
{
    return m_jsCtx;
}

void
Document::RemoveFromObjMap(JSObject *obj)
{
    m_documentObjectMap.erase(obj);
}

map <JSObject *, Document *> Document::m_documentObjectMap;

