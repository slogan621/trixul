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

#include "container.h"

Container::~Container()
{
    list <Element *>::iterator itr;
    list<Element *> children;

    children = GetChildren();

    for (itr = children.begin(); itr != children.end(); ++itr) 
        delete (*itr);
}

PRStatus Container::CreateChildren()
{
    // iterate the window's children, calling their create functions

    list <Element *>::iterator itr;
    WidgetImpl *thisImpl = GetImpl();

    if (!thisImpl) {  // box, grid don't have concrete impls
        Widget *w = dynamic_cast<Widget *>(this);
        WidgetImpl *impl;
        while ((w = dynamic_cast<Widget *>(w->GetParent())) != NULL) {
            impl = w->GetImpl();
            if (impl) {
                thisImpl = impl;
                break;
            } 
        }
    }

    if (!thisImpl) {
        return PR_FAILURE;
    }

    list<Element *> children;

    children = GetChildren();

    for (itr = children.begin(); itr != children.end(); ++itr) {
        WidgetImpl *impl;

#if 0
        impl = dynamic_cast <Widget *>(*itr)->GetImpl();
        impl->SetParent(thisImpl);
        dynamic_cast <Widget *>(*itr)->Create();
#else
        Widget *w;

        w = dynamic_cast <Widget *>(*itr);
        if (w) {
            impl = w->GetImpl();
            if (impl) {
                impl->SetParent(thisImpl);
            }
            w->Create();
        }
#endif
    }

    return PR_SUCCESS;
}       

PRStatus Container::DrawChildren()
{
    // iterate the window's children, calling their draw functions

    list <Element *>::iterator itr;
    list<Element *> children;

    children = GetChildren();

    for (itr = children.begin(); itr != children.end(); ++itr) {
        Widget *w;

        w = dynamic_cast <Widget *>(*itr);
        if (w) {
		    w->Draw();
        }
    }

	return PR_SUCCESS;
}

PRStatus Container::ShowChildren()
{
    // iterate the window's children, calling their show functions

    list <Element *>::iterator itr;
    list<Element *> children;

    children = GetChildren();

    for (itr = children.begin(); itr != children.end(); ++itr) {
        Widget *w;

        w = dynamic_cast <Widget *>(*itr);
        if (w) {
		    w->Show();
        }
    }

	return PR_SUCCESS;
}

PRStatus Container::HideChildren()
{
    // iterate the window's children, calling their hide functions

    list <Element *>::iterator itr;
    list<Element *> children;

    children = GetChildren();

    for (itr = children.begin(); itr != children.end(); ++itr) {
        Widget *w;

        w = dynamic_cast <Widget *>(*itr);
        if (w) {
            if (!w->IsContainer())
		        w->Hide();
            else {
                Container *c = reinterpret_cast<Container *>(w);
                if (c)
                    c->HideChildren();
            }
        }
    }

	return PR_SUCCESS;
}

void Container::PrintInformation() 
{
    int level = GetLevel();
    for(int i = 0;i < level;i++)
        cout << " ";
    cout << GetTypeName() << endl;
}



