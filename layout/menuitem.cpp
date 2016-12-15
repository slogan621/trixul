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

#include "menuitem.h"
#include "jsengine.h"
#include "plstr.h"

MenuItem::MenuItem() 
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_menuitem = factory->MakeMenuItem();
    if (m_menuitem) {
        RegisterListener();
    }
}

MenuItem::MenuItem(string& label)
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_menuitem = factory->MakeMenuItem();
    if (m_menuitem) {
	    SetLabel(label);
        RegisterListener();
    }
}

PRStatus MenuItem::GetValue(XPVariant &v)
{
    return PR_FAILURE;
}

PRStatus MenuItem::SetValue(const XPVariant &v)
{
    return PR_FAILURE;
}

MenuItem::~MenuItem()
{
    UnRegisterListener();
    if (m_menuitem)
        delete m_menuitem;
}

PRStatus MenuItem::SetLabel(string& label)
{
	AddAttribute(string("label"), label);

	if (m_menuitem)
		m_menuitem->SetLabel(label);
	return PR_SUCCESS;
}

string& MenuItem::GetLabel()
{
	AnAttribute *attribute;
	static string failret("");

	attribute = GetAttributeByName(string("label")); 
	if ( attribute ) 
		return attribute->GetValue();
	return failret;
}

MenuItemClass MenuItem::GetClass()
{
	AnAttribute *attribute;
    const char *classstring;
    MenuItemClass menuitemclass = MenuItemClassNone;

	attribute = GetAttributeByName(string("class")); 
	if (attribute) {
		classstring = attribute->GetValue().c_str();
        if (!PL_strcasecmp(classstring, "separator"))
            menuitemclass = MenuItemClassSeparator;
        else if (!PL_strcasecmp(classstring, "close"))
            menuitemclass = MenuItemClassClose;
        else if (!PL_strcasecmp(classstring, "save"))
            menuitemclass = MenuItemClassSave;
        else if (!PL_strcasecmp(classstring, "saveas"))
            menuitemclass = MenuItemClassSaveAs;
        else if (!PL_strcasecmp(classstring, "pagesetup"))
            menuitemclass = MenuItemClassPageSetup;
        else if (!PL_strcasecmp(classstring, "print"))
            menuitemclass = MenuItemClassPrint;
        else if (!PL_strcasecmp(classstring, "quit"))
            menuitemclass = MenuItemClassQuit;
        else if (!PL_strcasecmp(classstring, "cut"))
            menuitemclass = MenuItemClassCut;
        else if (!PL_strcasecmp(classstring, "copy"))
            menuitemclass = MenuItemClassCopy;
        else if (!PL_strcasecmp(classstring, "paste"))
            menuitemclass = MenuItemClassPaste;
        else if (!PL_strcasecmp(classstring, "clear"))
            menuitemclass = MenuItemClassClear;
        else if (!PL_strcasecmp(classstring, "selectall"))
            menuitemclass = MenuItemClassSelectAll;
        else if (!PL_strcasecmp(classstring, "preferences"))
            menuitemclass = MenuItemClassPreferences;
    }
    return menuitemclass;
}

PRStatus MenuItem::SetShortcut(string& shortcut)
{
    AddAttribute(string("shortcut"), shortcut);

    if (m_menuitem)
        m_menuitem->SetShortcut(shortcut);
    return PR_SUCCESS;
}

string& MenuItem::GetShortcut()
{
    AnAttribute *attribute;
    static string failret("");

    attribute = GetAttributeByName(string("shortcut"));
    if ( attribute )
        return attribute->GetValue();
    return failret;
}

PRStatus MenuItem::Draw()
{
	return PR_SUCCESS;
}

PRStatus MenuItem::Show()
{
    if (m_menuitem)
	    m_menuitem->Show();
    return PR_SUCCESS;
}

PRStatus MenuItem::Hide()
{
    if (m_menuitem)
	    m_menuitem->Show();
    return PR_SUCCESS;
}

PRStatus MenuItem::Enable()
{
    if (m_menuitem)
	    m_menuitem->Enable();
    SetEnabled(true);
    return PR_SUCCESS;
}

PRStatus MenuItem::Disable()
{
    if (m_menuitem)
	    m_menuitem->Disable();
    SetEnabled(false);
    return PR_SUCCESS;
}

PRStatus MenuItem::Create()
{    
    PRStatus status;

    m_menuitem->SetClass(GetClass());
    status = m_menuitem->Create();
    if (status == PR_SUCCESS) {
        status = SetLabel(GetLabel());
        status = SetShortcut(GetShortcut());
    }
    return status;
}

void MenuItem::MenuItemClicked()
{
	AnAttribute *attribute;
    string val;
    JSObject *obj;
    JSContext *cx;
    Document *doc = GetDocument();

    if (doc) {
        obj = doc->GetJSObject();
        cx = doc->GetJSCtx();
    }

	attribute = GetAttributeByName(string("onclick")); 
	if (attribute)  {
		val = attribute->GetValue();
        if (val.size()) {
            JS_BeginRequest(cx);
            JSFunction *func = JS_CompileFunction(cx, obj, "onclick",
                0, NULL, val.c_str(), val.size(),
                "internal", 1);
            if (func) {
                jsval rval;
                JSBool ret;
                ret = JS_CallFunction(cx, obj, func, 0, NULL, &rval);
                if (ret == JS_TRUE) {
                    printf("MenuItem::MenuItemClicked success\n");
                    if (JSVAL_IS_OBJECT(rval)) {
                        printf("MenuItem::MenuItemClicked. return is an object\n");
                    }
                    if (JSVAL_IS_NUMBER(rval)) {
                        printf("MenuItem::MenuItemClicked. return is a number\n");
                    }
                    if (JSVAL_IS_INT(rval)) {
                        printf("MenuItem::MenuItemClicked. return is an int\n");
                    }
                    if (JSVAL_IS_DOUBLE(rval)) {
                        printf("MenuItem::MenuItemClicked. return is a double\n");
                    }
                    if (JSVAL_IS_STRING(rval)) {
                        printf("MenuItem::MenuItemClicked. return is a string\n");
                    }
                    if (JSVAL_IS_BOOLEAN(rval)) {
                        printf("MenuItem::MenuItemClicked. return is a bool\n");
                    }
                    if (JSVAL_IS_NULL(rval)) {
                        printf("MenuItem::MenuItemClicked. return is a NULL\n");
                    }
                    if (JSVAL_IS_VOID(rval)) {
                        printf("MenuItem::MenuItemClicked. return is an void\n");
                    }
                    if (JSVAL_IS_PRIMITIVE(rval)) {
                        printf("MenuItem::MenuItemClicked. return is a primitive\n");
                    }
                    if (JSVAL_IS_INT(rval))
                        printf("MenuItem::MenuItemClicked return is %d\n",
                            JSVAL_TO_INT(rval));
                } else {
                    printf("MenuItem::MenuItemClicked fail\n");
                }
            }
            JS_EndRequest(cx);
        }
    }
    //if (js) 
        //js->PopContext();
}

PRStatus
MenuItem::RegisterListener()
{
    MenuItemClickSubject *pBPS =
        dynamic_cast <MenuItemClickSubject *>(m_menuitem);
    pBPS->Attach(this);
    return PR_SUCCESS;
}

PRStatus
MenuItem::UnRegisterListener()
{
    MenuItemClickSubject *pBPS =
        dynamic_cast <MenuItemClickSubject *>(m_menuitem);
    pBPS->Detach(this);
    return PR_SUCCESS;
}

PRStatus MenuItem::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_menuitem)
        m_menuitem->GetGeometry(x, y, width, height);

    return PR_SUCCESS;
}

PRStatus MenuItem::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char& mask)
{
    if (m_menuitem)
        m_menuitem->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus MenuItem::GetGeometryRequestImpl(int &x, int &y, int &width, int &height)
{
    if (m_menuitem)
        m_menuitem->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}

