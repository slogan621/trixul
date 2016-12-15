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

#if !defined( __DOCUMENT_H__ )
#define __DOCUMENT_H__

#include <list>
#include <map>
#include "element.h"

#include "script.h"
#include "jsengine.h"

#include "prettyprintvisitor.h"
#include "clipboardhelper.h"

class Window;

class Document {
public:
	Document(const string& path);
	~Document();
	Element *GetElementById(Element *root, const string& tag);
    static Document *FindDocumentByObj(JSObject *obj);
    static Window *FindWindowByObj(JSObject *obj);
	string& GetPath();
	void Dump(Element *root);
	void Dump();
	void SetRoot(Element *root);
	Element *GetRoot();
	void SetCurrent(Element *current);
	Element *GetCurrent();
	void SetLevel(int level);
	int GetLevel();
	Element *GetMainWindow(Element *root);
	Element *GetWindow(Element *root, bool isMain);
	PRStatus AddScript(Script *script);
	PRStatus ParseScripts();
	PRStatus ComputeLayout();
	PRStatus CloseScripts();
    PRStatus CreateJSObject();
    JSObject *GetJSObject();
    PRStatus AddDocumentToObjMap();
    static void RemoveFromObjMap(JSObject *obj);
    JSContext *GetJSCtx();
    void SetActive(Widget *widget) {m_helper.SetActive(widget);};
    void HandleClipboard(ClipboardHelper::ClipboardOp op) {m_helper.Handle(op);};
private:
    ClipboardHelper m_helper;
	string m_path;
	Element *m_root;
	Element *m_current;
	int m_level;
	list <Script *> m_scripts;
    JSContext *m_jsCtx;
    JSObject *m_jsObj;
    static map <JSObject *, Document *> m_documentObjectMap;
    AbstractElementVisitor *elementVisitor;
};

#endif
