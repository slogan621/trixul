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

#if !defined( __COMPONENT_H__ )
#define __COMPONENT_H__

#include <list>
#include <string>
#include <map>
#include "attributelist.h"

#include "prtypes.h"
#include "prlink.h"

#include "jsengine.h"

#include "library.h"

class Property : public AttributeList {
public:
};

class Arg : public AttributeList {
public:
};

class Return : public AttributeList {
public:
};

class Function : public AttributeList {
public:
    PRStatus AddArgument(Arg *arg) { m_args.push_back(arg); return PR_SUCCESS; };
    PRStatus SetReturn(Return *ret) { m_ret = ret; return PR_SUCCESS; };
    Return *GetReturn() { return m_ret; };
    list <Arg *> GetArgumentList() { return m_args; };
private:
    list <Arg *> m_args;
    Return *m_ret;
};

class Component;

typedef PRStatus (*CreatorFn)(JSObject *);
typedef PRStatus (*DestroyerFn)(JSObject *);

class Object : public AttributeList {
public:
    PRStatus AddFunction(Function *func) { m_funcs.push_back(func); return PR_SUCCESS; };
    PRStatus AddProperty(Property *prop) { m_props.push_back(prop); return PR_SUCCESS; };
    PRStatus SetCurFunction(Function *func) { m_curFunc = func; return PR_SUCCESS; };
    Function *GetCurFunction() { return m_curFunc; };
    list <Property *> GetPropertyList() { return m_props; };
    list <Function *> GetFunctionList() { return m_funcs; };

    PRStatus CreateJSObject();
    JSObject *GetJSObject();
    string& GetGUUID();
    PRStatus SetComponent(Component *component);
    Component *GetComponent();
    PRStatus CallDestroyer();

private:
    Component *m_component; // the parent component
    Function *m_curFunc;
    JSObject *m_jsObj;
    list <Function *> m_funcs;
    list <Property *> m_props;
    string m_name;
    JSClass m_jsClass; 
    DestroyerFn m_pDestroyer;
};

class Component : public AttributeList {
public:
	Component(const string& path);
	string& GetPath();  // set in CTOR
	string& GetProxyPath(); 
	string& GetFile(); 
    PRStatus AddClass(Object *obj);
    PRStatus SetCurClass(Object *obj) { m_curObject = obj; return PR_SUCCESS; };
    Object *GetCurClass() { return m_curObject; };
    PRStatus CreateProxyHeader(const string& path); 
    PRStatus CreateProxySource(const string& headerPath, const string& sourcePath, const string& componentHeaderPath); 
    PRStatus CreateComponentHeader(const string& path); 
    bool GetIsLoaded(); 
    PRStatus SetIsLoaded(bool isloaded); 
    string& GetGUUID();
    Library *GetLibrary();
    PRStatus SetLibrary(Library *lib);
    Library *GetProxyLibrary();
    PRStatus SetProxyLibrary(Library *lib);

    typedef enum {
        PRType_Unknown,
        PRType_PRInt8,
        PRType_PRInt16,
        PRType_PRUint16,
        PRType_PRInt32,
        PRType_PRUint32,
        PRType_PRInt64,
        PRType_PRUint64,
        PRType_PRFloat64,
        PRType_PRSize,
        PRType_PROffset32,
        PRType_PROffset64,
        PRType_PRBool,
        PRType_PRString
    } PR_Type;

    PR_Type StrToPRType(const char *str);
    Object *GetObjectByGUUID(const string& guuid);
    Object *GetObjectByJSObject(JSObject *obj);
    PRStatus CreateJSObject();
    JSObject *GetJSObject();

private:
	string m_path;
	string m_proxyPath;
    Object *m_curObject;
    Library *m_lib;
    Library *m_proxyLib;
    bool m_isLoaded;
    list <Object *> m_objects;
    map <string, Object *> m_objectsByGUUID;
    JSObject *m_jsObj;
    JSClass m_jsClass; 
};

#endif
