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

#include <stdio.h>

#include "layout.h"

#include "button.h"
#include "menu.h"
#include "menubar.h"
#include "menuitem.h"
#include "text.h"
#include "statictext.h"
#include "window.h"
#include "box.h"
#include "spacer.h"
#include "scrolledwindow.h"
#include "scrolledview.h"
#include "gridlist.h"
#include "radiobutton.h"
#include "checkbox.h"
#include "app.h"

#include "componentmgr.h"
#include "filepicker.h"

#include "script.h"

#include "expat.h"

#include <iostream>
#include <fstream>

#include <prthread.h>
#include <plstr.h>

#include <plgetopt.h>

static void StartElement(void *userData, const char *name, const char **atts);
static void EndElement(void *userData, const char *name);

Layout * 
Layout::GetInstance()
{
    static Layout *instance = NULL;

    if (instance == NULL)
        instance = new Layout();
    return instance;
}

Layout::Layout() // : m_jsObj(NULL)
{
    m_documents.clear();
    JSEngine *js = JSEngine::GetJSEngine();
    if (js)
        js->InitEngine();
}

Layout::~Layout()
{
}

void
Layout::Shutdown()
{
    list <Document *>::iterator iter;

    for (iter = m_documents.begin(); iter != m_documents.end(); ++iter)
        delete(*iter);
    m_documents.clear();
    JSEngine *js = JSEngine::GetJSEngine();
    if (js)
        js->ReleaseEngine();
}

void
Layout::RemoveDocumentFromMap(Document *doc)
{
    list <Document *>::iterator iter;

    for (iter = m_documents.begin(); iter != m_documents.end(); ++iter) {
        if (*iter == doc) {
            delete(*iter);
            m_documents.erase(iter);
            break;
        }
    }
}

static int
StubHandleExternalEntityRef(XML_ParserStruct* aExternalEntityRefHandler,
    const XML_Char *openEntityNames,
    const XML_Char *base,
    const XML_Char *systemId,
    const XML_Char *publicId)
{
    Layout *layout = reinterpret_cast<Layout *>(aExternalEntityRefHandler);
    if (layout)
        return layout->HandleExternalEntityRef(aExternalEntityRefHandler,    
            openEntityNames, base, systemId, publicId);
    else
        return 1;
}

int
Layout::HandleExternalEntityRef(
    XML_ParserStruct* aExternalEntityRefHandler,
    const XML_Char *openEntityNames,
    const XML_Char *base,
    const XML_Char *systemId,
    const XML_Char *publicId)
{
    if (!systemId || !aExternalEntityRefHandler)
        return 1;

    string prefix("resources/locale/en_US/"); // XXX
    string path(systemId);
    path = prefix + path;
    ifstream from(path.c_str());

    if (!from.is_open())
        return 1;
    XML_Parser entityParser = XML_ExternalEntityParserCreate(
        m_parser, NULL, NULL);
    if (entityParser) {
        PRInt32 done = 0;
        char buf[BUFSIZ];
        while (!from.eof()) {
            from.getline(buf, BUFSIZ);
            done = from.eof();
            if (XML_Parse(entityParser, buf, strlen(buf), done) == 
                XML_STATUS_ERROR) {
                fprintf(stderr,       // XXX
                    "%s at line %d\n",
                    XML_ErrorString(XML_GetErrorCode(entityParser)),
                    XML_GetCurrentLineNumber(entityParser));
                break;
            }
        }
        XML_ParserFree(entityParser);
    }
    return 1;
}

static void
StartElement(void *userData, const char *name, const char **atts)
{
    Document *document = (Document *) userData;
    Widget *w = NULL;
    Script *script = NULL;
    int i;

    if (!PL_strcasecmp(name, "button")) {
        w = new Button();
        w->SetType(TYPE_BUTTON);
    }
    else if (!PL_strcasecmp(name, "gridlist")) {
        w = new GridList();
        w->SetType(TYPE_GRIDLIST);
    }
    else if (!PL_strcasecmp(name, "radiobutton")) {
        w = new RadioButton();
        w->SetType(TYPE_RADIOBUTTON);
    }
    else if (!PL_strcasecmp(name, "checkbox")) {
        w = new CheckBox();
        w->SetType(TYPE_CHECKBOX);
    }
    else if (!PL_strcasecmp(name, "menu")) {
        w = new Menu();
        w->SetType(TYPE_MENU);
    }
    else if (!PL_strcasecmp(name, "menubar")) {
        w = new Menubar();
        w->SetType(TYPE_MENUBAR);
    }
    else if (!PL_strcasecmp(name, "menuitem")) {
        w = new MenuItem();
        w->SetType(TYPE_MENUITEM);
    }
    else if (!PL_strcasecmp(name, "scrolledwindow")) { 
        w = new ScrolledWindow();
        if (w) {
            w->SetType(TYPE_SCROLLEDWINDOW);
        }
    }
    else if (!PL_strcasecmp(name, "scrolledview")) { 
        w = new ScrolledView();
        if (w) {
            w->SetType(TYPE_SCROLLEDVIEW);
        }
    }
    else if (!PL_strcasecmp(name, "window")) { 
        w = new Window();
        if (w) {
            w->SetType(TYPE_WINDOW);
        }
    }
    else if (!PL_strcasecmp(name, "box")) { 
        w = new Box();
        w->SetType(TYPE_BOX);
    }
    else if (!PL_strcasecmp(name, "text")) { 
        w = new Text();
        w->SetType(TYPE_TEXT);
    }
    else if (!PL_strcasecmp(name, "spacer")) { 
        w = new Spacer();
        w->SetType(TYPE_SPACER);
    }
    else if (!PL_strcasecmp(name, "statictext")) { 
        w = new StaticText();
        w->SetType(TYPE_STATIC_TEXT);
    }
    else if (!PL_strcasecmp(name, "script")) { 
        script = new Script();
    }

    if (w) {
        Widget *p = dynamic_cast<Widget *>(document->GetCurrent());
        w->SetParent(document->GetCurrent());

        if (p) {
            WidgetImpl *wimpl, *pimpl;

            wimpl = w->GetImpl();
            pimpl = p->GetImpl();
            if (wimpl && pimpl) {
                wimpl->SetParent(pimpl);
            }
            if (wimpl) {
                wimpl->SetDocument(document);
                wimpl->SetWidget(w);
            }
        }
        w->SetDocument(document);
        document->GetCurrent()->AddChild(w, document->GetLevel());
        if (w->IsContainer()) {
            document->SetCurrent(w);
            document->SetLevel(document->GetLevel() + 1);
        }

        for (i = 0; atts[i]; i += 2) {
            w->AddAttribute(string(atts[i]), string(atts[i+1]));
        }

    } else if (script) {
        for (i = 0; atts[i]; i += 2) {
            script->AddAttribute(string(atts[i]), string(atts[i+1]));
        }
        document->AddScript(script);
    }
}

static bool 
IsContainer(const char *name)
{
    bool ret = false;
    if (!PL_strcasecmp(name, "window"))
        ret = true;
    else if (!PL_strcasecmp(name, "hbox"))
        ret = true;
    else if (!PL_strcasecmp(name, "vbox"))
        ret = true;
    else if (!PL_strcasecmp(name, "box"))
        ret = true;
    else if (!PL_strcasecmp(name, "menubar"))
        ret = true;
    else if (!PL_strcasecmp(name, "menu"))
        ret = true;
    else if (!PL_strcasecmp(name, "scrolledwindow"))
        ret = true;
    else if (!PL_strcasecmp(name, "scrolledview"))
        ret = true;
    return ret;
}

static void
EndElement(void *userData, const char *name)
{
    if (PL_strcasecmp(name, "script")) {  // XXX or StartElement failed

        Document *document = (Document *) userData;

        if (IsContainer(name)) {
            // unwind the parent stack

            document->SetCurrent(document->GetCurrent()->GetParent());
            document->SetLevel(document->GetLevel() - 1);
        }
    }
}

Document *
Layout::ProcessFile(const string& path)
{
    // XXX check to see if we already have a DOM for this document. 
    // if so, return it

    ifstream from(path.c_str());

    if (!from.is_open()) {
        return NULL;   
    }

    Document *document = new Document(path);

    if (!document) {
        return NULL;
    }

    document->SetLevel(0);

    char buf[BUFSIZ];
    m_parser = XML_ParserCreate(NULL);
    if (!m_parser) {
        delete document; 
        return NULL;
    }

    PRInt32 done = 0;
    XML_SetUserData(m_parser, document);
    XML_SetElementHandler(m_parser, StartElement, EndElement);
    XML_SetParamEntityParsing(m_parser, XML_PARAM_ENTITY_PARSING_ALWAYS);
    XML_SetExternalEntityRefHandler(m_parser, StubHandleExternalEntityRef);
    XML_SetExternalEntityRefHandlerArg(m_parser, this);

    while (!from.eof()) {
        from.getline(buf, BUFSIZ);
        done = from.eof();
        if (XML_Parse(m_parser, buf, strlen(buf), done) == XML_STATUS_ERROR) {
            fprintf(stderr,       // XXX
                "%s at line %d\n",
                XML_ErrorString(XML_GetErrorCode(m_parser)),
                XML_GetCurrentLineNumber(m_parser));
            delete document;
            XML_ParserFree(m_parser);      
            return NULL;
        }
    }
    m_documents.push_back(document);
    XML_ParserFree(m_parser);      

    document->CreateJSObject();
    return document;
}

#if defined(XP_WIN)
int main(int argc, char *argv[])
#else
int LayoutMain(int argc, char *argv[])
#endif
{
    int run, parse;
    char *src = NULL;
    char *proxyHeaderOut = "", *proxySourceOut = "", *componentHeaderOut = "";
    void usage();


    run = parse = 0;
    PLOptState *state;
    PLOptStatus os;
    state = PL_CreateOptState(argc, argv, "w:h:s:c:r:p:");
    if (!state)
        usage();
    while ((os = PL_GetNextOpt(state)) == PL_OPT_OK) {
        if (PL_OPT_BAD == os) continue;
        switch (state->option) {
            case 'r':
                run = 1;
                src = const_cast<char *>(state->value); 
                break;
            case 'p':
                parse = 1;
                src = const_cast<char *>(state->value); 
                break;
            case 'w':
                proxyHeaderOut = const_cast<char *>(state->value); 
                break;
            case 's':
                proxySourceOut = const_cast<char *>(state->value); 
                break;
            case 'c':
                componentHeaderOut = const_cast<char *>(state->value); 
                break;
            case 'h':
                usage();
                break;
            default:
                usage();
        }
    }
    PL_DestroyOptState(state);

    if (parse) {
        ComponentManager *cmgr = ComponentManager::GetInstance();

        cmgr->ParseSIL(src,
            proxyHeaderOut, proxySourceOut, componentHeaderOut);
    }
    else if (run) {
        Document *document;
        Layout *layout = Layout::GetInstance();
        document = layout->ProcessFile(src);
        if (document) {
#if 1
            document->Dump();
#endif
            // create an application context

            App *app = App::GetAppInstance();
            if (app) {

                // initialize the application

                app->Initialize(&argc, argv);

                JSEngine *js = JSEngine::GetJSEngine();
                if (js)
                    js->SetApp(app);

                // load components

                ComponentManager *cmgr = ComponentManager::GetInstance();

                if (cmgr) {
                    cmgr->CreateJSObject();
                    cmgr->ProcessComponents();
                }

                FilePicker *fpicker = FilePicker::GetInstance();

                if (fpicker) 
                    fpicker->CreateJSObject();

                // parse any scripts associated with the doc

                document->ParseScripts();
    
                // find the element that is a window and has main="true"
                // as an attribute, and show it

                Window *w;

                w = reinterpret_cast<Window *>(document->GetMainWindow(NULL));

                if (w && w->Create() == PR_SUCCESS) {
                    // compute layouts of all widgets

                    document->ComputeLayout();
                    w->Show();
                    w->OnLoad();
                }
                app->MainLoop();
            }
        }
        layout->Shutdown(); 
    }
    return 0;
}

void usage() 
{
    std::cout << "usage: layout [-p sil -w proxyheader -s proxysrc -c componenthdr -h] [-r doc]\n";
    exit(1);
}

