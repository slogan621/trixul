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

#include "component.h"
#include "componentmgr.h"

#include <iostream>

using namespace std;

#include "plbasename.h" 

#define PREAMBLE "/* This is generated source code. Do not edit */\n\n"

#define EXPORTHDR "#if defined(WIN32)\n#define EXPORT __declspec(dllexport)\n#else\n#define EXPORT\n#endif\n\n"

Component::Component(const string& path) : m_path(path), m_lib(NULL), 
    m_proxyLib(NULL), m_isLoaded(false)
{
    JSClass component_class = {
        "component", 0,
        JS_PropertyStub,  JS_PropertyStub,
        JS_PropertyStub,  JS_PropertyStub,
        JS_EnumerateStub,  JS_ResolveStub,
        JS_ConvertStub,   JS_FinalizeStub
    };

    m_jsClass = component_class;
}

bool 
Component::GetIsLoaded() 
{
    return m_isLoaded;
}

PRStatus 
Component::SetIsLoaded(bool isLoaded) 
{
    m_isLoaded = isLoaded;
    return PR_SUCCESS;
}

PRStatus 
Component::AddClass(Object *obj) 
{ 
    obj->SetComponent(this);
    m_objects.push_back(obj); 
    string guuid = obj->GetGUUID();
    if (guuid.size()) {
        m_objectsByGUUID[guuid] = obj;
        return PR_SUCCESS; 
    } 
    return PR_FAILURE;
}

Library * 
Component::GetLibrary() 
{
    return m_lib;
}

PRStatus 
Component::SetLibrary(Library *lib) 
{
    m_lib = lib;
    return PR_SUCCESS;
}

Library * 
Component::GetProxyLibrary() 
{
    return m_proxyLib;
}

PRStatus 
Component::SetProxyLibrary(Library *lib) 
{
    m_proxyLib = lib;
    return PR_SUCCESS;
}

string& Component::GetGUUID()
{
    AnAttribute *attribute;
    static string failret("");

    attribute = GetAttributeByName(string("id"));
    if (attribute)
        return attribute->GetValue();
    return failret;
}

string& Component::GetFile()
{
    AnAttribute *attribute;
    static string failret("");

    attribute = GetAttributeByName(string("name"));
    if (attribute)
        return attribute->GetValue();
    return failret;
}

string& Component::GetPath()
{
	return m_path;
}

string& Component::GetProxyPath()
{
	return m_proxyPath;
}

PRStatus Component::CreateProxyHeader(const string& path)
{
    string guuid = GetGUUID();

    for (unsigned int i = 0; i < guuid.size(); i++) {
        if (guuid[i] == '-')
            guuid[i] = '_';
    }

    ofstream to(path.c_str());
    
    if (!to.is_open())
        return PR_FAILURE;

    to << PREAMBLE;

    to << "#if !defined(__" << guuid << "_PROXY_H__)\n";
    to << "#define __" << guuid << "_PROXY_H__\n\n";

    to << EXPORTHDR;;

    to << "#include <prtypes.h>\n#include \"jsapi.h\"\n\n";

    // iterate classes

    list <Object *>::iterator objiter;

    for (objiter = m_objects.begin(); objiter != m_objects.end(); ++objiter) {
        Object *obj = *objiter;

        AnAttribute *attr;

        attr = obj->GetAttributeByName("name");

        if (!attr) 
            return PR_FAILURE;

        list <Property *> props = obj->GetPropertyList();
        list <Property *>::iterator propiter; 
        for (propiter = props.begin(); propiter != props.end(); ++propiter) {
            Property *prop = *propiter;

            AnAttribute *typeattr, *nameattr;
            
            typeattr = prop->GetAttributeByName("type");
            nameattr = prop->GetAttributeByName("name");

            if (typeattr && nameattr) {
                to <<  "extern \"C\" EXPORT JSBool\n" 
                    << attr->GetValue().c_str() 
                    << "_Get_" 
                    << nameattr->GetValue().c_str() 
                    << "(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);\n\n";
                to <<  "extern \"C\" EXPORT JSBool\n" 
                    << attr->GetValue().c_str() 
                    << "_Set_" 
                    << nameattr->GetValue().c_str() 
                    << "(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);\n\n";
            }
        }

        list <Function *> funcs = obj->GetFunctionList();
        list <Function *>::iterator funciter; 
        for (funciter = funcs.begin(); funciter != funcs.end(); ++funciter) {
            Function *func = *funciter;
          
            AnAttribute *nameattr, *typeattr;
            
            Return * ret = func->GetReturn();
            typeattr = ret->GetAttributeByName("type");

            if (typeattr) {
                to << "extern \"C\" EXPORT JSBool\n";
            }

            nameattr = func->GetAttributeByName("name");

            if (nameattr) {
                to << attr->GetValue().c_str() << "_"
                    << nameattr->GetValue().c_str() 
                    << "(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);\n\n"; 
            }
        }

#if 0
        for (propiter = props.begin(); propiter != props.end(); ++propiter) {
            Property *prop = *propiter;

            AnAttribute *typeattr, *nameattr;
            
            typeattr = prop->GetAttributeByName("type");
            nameattr = prop->GetAttributeByName("name");

            if (typeattr && nameattr) {
                printf("    %s m_%s;\n", 
                    typeattr->GetValue().c_str(),
                    nameattr->GetValue().c_str());
            }
        }
#endif
    
        to << "extern \"C\" EXPORT PRStatus New" << attr->GetValue().c_str() << "Proxy(JSObject *obj);\n\n";
        to << "extern \"C\" EXPORT PRStatus Delete" << attr->GetValue().c_str() << "Proxy(JSObject *obj);\n\n";
    }

    to << "#endif\n";
    return PR_SUCCESS;
}

PRStatus Component::CreateProxySource(const string& headerPath, 
    const string& sourcePath, const string& componentHeaderPath)
{
    string base(PL_basename(const_cast<char *>(headerPath.c_str())));
    string base2(PL_basename(const_cast<char *>(componentHeaderPath.c_str())));
    ofstream to(sourcePath.c_str());
    
    if (!to.is_open())
        return PR_FAILURE;

    to << PREAMBLE;

    // iterate classes
    
    to << "#include \"" << base << "\"\n";
    to << "#include \"" << base2 << "\"\n";

    to << "#include \"prtypes.h\"\n";
    to << "#include \"jsstr.h\"\n";
    to << "#include <map>\n\n";
    to << "using namespace std;\n\n";

    list <Object *>::iterator objiter;

    for (objiter = m_objects.begin(); objiter != m_objects.end(); ++objiter) {
        Object *obj = *objiter;

        AnAttribute *attr;

        attr = obj->GetAttributeByName("name");

        to << "static map<JSObject *, " << attr->GetValue().c_str() << "*> obj"
            << attr->GetValue().c_str() << "Map;\n\n";

        to << "static " << attr->GetValue().c_str() << " *\n";
        to << "Get" << attr->GetValue().c_str() << "Class(JSObject *obj)\n";
        to << "{\n    return obj" << attr->GetValue().c_str() << "Map[obj];\n}\n\n"; 

        to << "static void\n";
        to << "Add" << attr->GetValue().c_str() << "Class(JSObject *obj, ";
        to << attr->GetValue().c_str()  << " *cPtr)\n";
        to << "{\n    obj" << attr->GetValue().c_str() << "Map[obj] = cPtr;\n}\n\n"; 

        to << "static void\n";
        to << "Remove" << attr->GetValue().c_str() << "Class(JSObject *obj)\n";
        to << "{\n    obj" << attr->GetValue().c_str() << "Map.erase(obj);\n}\n\n"; 

        to << "extern \"C\" EXPORT PRStatus\n";
        to << "New" << attr->GetValue().c_str() << "Proxy(JSObject *obj)\n";
        to << "{\n";
        to << "    " << attr->GetValue().c_str() << " *ptr = new " << attr->GetValue().c_str() << "();\n";
        to << "    if (!ptr)\n        return PR_FAILURE;\n";
        to << "    Add" << attr->GetValue().c_str() << "Class(obj, ptr);\n"; 
        to << "    return PR_SUCCESS;\n}\n\n";

        to << "extern \"C\" EXPORT PRStatus\n";
        to << "Delete" << attr->GetValue().c_str() << "Proxy(JSObject *obj)\n";
        to << "{\n";
        to << "    " << attr->GetValue().c_str() << " *ptr;\n";
        to << "    if (obj) {\n";
        to << "        ptr = Get" << attr->GetValue().c_str() << "Class(obj);\n";
        to << "        Remove" << attr->GetValue().c_str() << "Class(obj);\n";
        to << "        delete ptr;\n    }\n    return PR_SUCCESS;\n}\n\n";

        list <Function *> funcs = obj->GetFunctionList();
        list <Function *>::iterator funciter;
        for (funciter = funcs.begin(); funciter != funcs.end(); ++funciter) {
            Function *func = *funciter;
          
            AnAttribute *nameattr, *retTypeattr;
            
            Return * ret = func->GetReturn();
            retTypeattr = ret->GetAttributeByName("type");

            to << "extern \"C\" EXPORT JSBool\n";

            nameattr = func->GetAttributeByName("name");

            string funcname;
            if (nameattr) {
                funcname = nameattr->GetValue();
                to << attr->GetValue().c_str() << "_" <<
                    funcname.c_str() << 
                    "(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)\n{\n";
            }

            to << "    " << attr->GetValue().c_str() << " *ptr = Get" << attr->GetValue().c_str() << "Class(obj);\n";
            to << "    if (!ptr)\n        return JS_FALSE;\n";

            // convert the arguments, and then call the implementation

            list <Arg *> args = func->GetArgumentList();
            list <Arg *>::iterator argiter;
            int i;
            for (i=0, argiter = args.begin(); argiter != args.end(); i++, ++argiter) {

                Arg *arg = *argiter;

                nameattr = func->GetAttributeByName("name");
                AnAttribute *typeattr = arg->GetAttributeByName("type");

                if (typeattr && nameattr) {
                    PR_Type val = StrToPRType(typeattr->GetValue().c_str());
                    switch(val) {
                        case PRType_PRInt8:
                            to << "    PRInt8 arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PRInt16:
                            to << "    PRInt16 arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PRUint16:
                            to << "    PRUint16 arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PRInt32:
                            to << "    PRInt32 arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PRUint32:
                            to << "    PRUint32 arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PRInt64:
                            to << "    PRInt64 arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PRUint64:
                            to << "    PRUint64 arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PRFloat64:
                            to << "    PRFloat64 arg_" << i << " = JSVAL_TO_DOUBLE(argv[" << i << "]);\n";
                            break;
                        case PRType_PRSize:
                            to << "    PRSize arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PROffset32:
                            to << "    PROffset32 arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PROffset64:
                            to << "    PROffset64 arg_" << i << " = JSVAL_TO_INT(argv[" << i << "]);\n";
                            break;
                        case PRType_PRBool:
                            to << "    PRBool arg_" << i << " = JSVAL_TO_BOOLEAN(argv[" << i << "]);\n";
                            break;
                        case PRType_PRString:
                            to << "    JSString *str_" << i << " = JSVAL_TO_STRING(argv[" << i << "]);\n";
                            to << "    string arg_" << i << "(JS_GetStringBytes(str_" << i << "));\n";
                            break;
                        default:
                            break;
                    }
                }
            }

            bool hasRet = false;
            string retString, convertString;
            PR_Type retval = StrToPRType(retTypeattr->GetValue().c_str());
            if (retval != PRType_Unknown) {
                hasRet = true;
                switch(retval) {
                    case PRType_PRInt8:
                        retString = "PRInt8"; 
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PRInt16:
                        retString = "PRInt16";
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PRUint16:
                        retString = "PRUint16";
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PRInt32:
                        retString = "PRInt32";
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PRUint32:
                        retString = "PRUint32";
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PRInt64:
                        retString = "PRInt64"; 
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PRUint64:
                        retString = "PRUint64"; 
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PRFloat64:
                        retString = "PRFloat64"; 
                        convertString = "DOUBLE_TO_JSVAL";
                        break;
                    case PRType_PRSize:
                        retString = "PRSize"; 
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PROffset32:
                        retString = "PROffset32"; 
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PROffset64:
                        retString = "PROffset64"; 
                        convertString = "INT_TO_JSVAL";
                        break;
                    case PRType_PRBool:
                        retString = "PRBool"; 
                        convertString = "BOOLEAN_TO_JSVAL";
                        break;
                    case PRType_PRString:
                        retString = "string"; 
                        convertString = "STRING_TO_JSVAL";
                        break;
                    default:
                        break;
                }
            }
            if (hasRet) {
                to << "    " << retString << " ret;\n";
                to << "    ret = ptr->";
            }
            else
                to << "    ptr->";
            to << funcname.c_str() << "(";
            for (i = 0, argiter = args.begin(); argiter != args.end(); i++, ++argiter) {
                if (i != 0) {
                    to << ", ";
                }

                to << "arg_" << i;
            }
            to << ");\n";
            if (hasRet) {
                if (retval == PRType_PRString) {
                    to << "    JSString *str = ::JS_NewStringCopyN(cx, ret.c_str(), ret.size() + 1);\n";
                    to << "    *rval = " << convertString << "(str);\n"; 
                } else
                    to << "    *rval = " << convertString << "(ret);\n"; 
            }
            to << "    return JS_TRUE;\n";
            to << "}\n\n";
        }

        list <Property *> props = obj->GetPropertyList();
        list <Property *>::iterator propiter;
        for (propiter = props.begin(); propiter != props.end(); ++propiter) {
            Property *prop = *propiter;

            AnAttribute *typeattr, *nameattr;
            
            typeattr = prop->GetAttributeByName("type");
            nameattr = prop->GetAttributeByName("name");

            string typestring(""), macro;

            PR_Type val;
            if (typeattr) {
                val = StrToPRType(typeattr->GetValue().c_str());
                switch(val) {
                    case PRType_PRInt8:
                        typestring = "PRInt8"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PRInt16:
                        typestring = "PRInt16"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PRUint16:
                        typestring = "PRUint16"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PRInt32:
                        typestring = "PRInt32"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PRUint32:
                        typestring = "PRUint32"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PRInt64:
                        typestring = "PRInt64"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PRUint64:
                        typestring = "PRUint64"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PRFloat64:
                        typestring = "PRFloat64"; macro = "JSVAL_TO_DOUBLE";
                        break;
                    case PRType_PRSize:
                        typestring = "PRSize"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PROffset32:
                        typestring = "PROffset32"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PROffset64:
                        typestring = "PROffset64"; macro = "JSVAL_TO_INT";
                        break;
                    case PRType_PRBool:
                        typestring = "PRBool"; macro = "JSVAL_TO_BOOLEAN";
                        break;
                    case PRType_PRString:
                        typestring = "string"; macro = "STRING_TO_JSVAL";
                        break;
                    default:
                        break;
                }
            }

            if (typeattr && nameattr && typestring.length()) {
                to <<  "extern \"C\" EXPORT JSBool\n" << attr->GetValue().c_str() << "_Get_"
                    << nameattr->GetValue().c_str() 
                    << "(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)\n{\n";
                to << "    " << typestring << " arg_0;\n";
                to << "    " << attr->GetValue().c_str() << " *ptr = Get" << attr->GetValue().c_str() << "Class(obj);\n";
                to << "    if (!ptr || ptr->Get_" << nameattr->GetValue().c_str() << "(arg_0) != PR_SUCCESS)\n";
                to << "        return JS_FALSE;\n";
                if (val == PRType_PRString) {
                    to << "    JSString *str = ::JS_NewStringCopyN(cx, arg_0.c_str(), arg_0.size() + 1);\n";
                    to << "    *rval = " << macro << "(str);\n";
                } else {
                    to << "    *rval = " << macro << "(arg_0);\n";
                }
                to << "    return JS_TRUE;\n";
                to << "}\n\n";

                to <<  "extern \"C\" EXPORT JSBool\n" << attr->GetValue().c_str() << "_Set_"
                    << nameattr->GetValue().c_str() 
                    << "(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)\n{\n";
                if (val == PRType_PRString) {
                    to << "    JSString *str_0 = JSVAL_TO_STRING(argv[0]);\n";
                    to << "    string arg_0(JS_GetStringBytes(str_0));\n";
                } else {
                    to << "    " << typestring << " arg_0 = " << macro << "(argv[0]);\n";
                }
                to << "    " << attr->GetValue().c_str() << " *ptr = Get" << attr->GetValue().c_str() << "Class(obj);\n";
                to << "    if (!ptr || ptr->Set_" << nameattr->GetValue().c_str() << "(arg_0) != PR_SUCCESS)\n";
                to << "        return JS_FALSE;\n";
                to << "    return JS_TRUE;\n";
                to << "}\n\n";
            }
        }
    }
    return PR_SUCCESS;
}

PRStatus Component::CreateComponentHeader(const string& path)
{
    ofstream to(path.c_str());
    
    if (!to.is_open())
        return PR_FAILURE;

    to << PREAMBLE;

    string guuid = GetGUUID();

    for (unsigned int i = 0; i < guuid.size(); i++) {
        if (guuid[i] == '-')
            guuid[i] = '_';
    }

    to << "#if !defined(__" << guuid << "_COMP_H__)\n";
    to << "#define __" << guuid << "_COMP_H__\n\n";

    to << "#include \"prtypes.h\"\n";
    to << "#include <string>\n";
    to << "using namespace std;\n";

    // iterate classes

    list <Object *>::iterator objiter;

    for (objiter = m_objects.begin(); objiter != m_objects.end(); ++objiter) {
        Object *obj = *objiter;

        AnAttribute *attr;

        attr = obj->GetAttributeByName("name");

        if (!attr) 
            return PR_FAILURE;

        to << "class " << attr->GetValue().c_str() << " {\npublic:\n";
        to << "    " << attr->GetValue().c_str() << "();\n";
        to << "    ~" << attr->GetValue().c_str() << "();\n";

        list <Property *> props = obj->GetPropertyList();
        list <Property *>::iterator propiter; 
        for (propiter = props.begin(); propiter != props.end(); ++propiter) {
            Property *prop = *propiter;

            AnAttribute *typeattr, *nameattr;
            
            typeattr = prop->GetAttributeByName("type");
            nameattr = prop->GetAttributeByName("name");

            if (typeattr && nameattr) {
                to <<  "    PRStatus Get_" << nameattr->GetValue().c_str() <<
                       "(" << typeattr->GetValue().c_str() <<
                       "& val) {val = m_" << nameattr->GetValue().c_str() <<
                       "; return PR_SUCCESS;};\n";
                to <<  "    PRStatus Set_" << nameattr->GetValue().c_str() <<
                       "(" << typeattr->GetValue().c_str() <<
                       "& val) {m_" << nameattr->GetValue().c_str() <<
                       " = val; return PR_SUCCESS;};\n";
            }
        }

        list <Function *> funcs = obj->GetFunctionList();
        list <Function *>::iterator funciter; 
        for (funciter = funcs.begin(); funciter != funcs.end(); ++funciter) {
            Function *func = *funciter;
          
            AnAttribute *nameattr, *typeattr;
            
            Return * ret = func->GetReturn();
            typeattr = ret->GetAttributeByName("type");

            if (typeattr) {
                to <<   "    " << typeattr->GetValue().c_str() << " ";
            }

            nameattr = func->GetAttributeByName("name");

            if (nameattr) {
                to << nameattr->GetValue().c_str() << "(";
            }
   
            list <Arg *> args = func->GetArgumentList();
            list <Arg *>::iterator argiter; 
            for (argiter = args.begin(); argiter != args.end(); ++argiter) {

                if (argiter != args.begin())
                    to << ", ";
                Arg *arg = *argiter;

                typeattr = arg->GetAttributeByName("type");

                if (typeattr) {
                    to << typeattr->GetValue().c_str() << "& ";
                }

                nameattr = arg->GetAttributeByName("name");

                if (nameattr) {
                    to << nameattr->GetValue().c_str();
                }
            }
            to << ");\n";
        }

        to << "private:\n";

        for (propiter = props.begin(); propiter != props.end(); ++propiter) {
            Property *prop = *propiter;

            AnAttribute *typeattr, *nameattr;
            
            typeattr = prop->GetAttributeByName("type");
            nameattr = prop->GetAttributeByName("name");

            if (typeattr && nameattr) {
                to << "    " << typeattr->GetValue().c_str() <<
                " m_" << nameattr->GetValue().c_str() << ";\n"; 
            }
        }
   
        to << "};\n\n";
    }
    to << "#endif\n\n";

    return PR_SUCCESS;
}

Component::PR_Type
Component::StrToPRType(const char *str)
{
    PR_Type ret = PRType_Unknown;

    if (!strcmp(str, "PRInt8")) {
        ret = PRType_PRInt8;
    } else if (!strcmp(str, "PRInt16")) {
        ret = PRType_PRInt16;
    } else if (!strcmp(str, "PRUint16")) {
        ret = PRType_PRUint16;
    } else if (!strcmp(str, "PRInt32")) {
        ret = PRType_PRInt32;
    } else if (!strcmp(str, "PRUint32")) {
        ret = PRType_PRUint32;
    } else if (!strcmp(str, "PRInt64")) {
        ret = PRType_PRInt64;
    } else if (!strcmp(str, "PRUInt64")) {
        ret = PRType_PRUint64;
    } else if (!strcmp(str, "PRFloat64")) {
        ret = PRType_PRFloat64;
    } else if (!strcmp(str, "PRSize")) {
        ret = PRType_PRSize;
    } else if (!strcmp(str, "PROffset32")) {
        ret = PRType_PROffset32;
    } else if (!strcmp(str, "PROffset64")) {
        ret = PRType_PROffset64;
    } else if (!strcmp(str, "PRBool")) {
        ret = PRType_PRBool;
    } else if (!strcmp(str, "string")) {
        ret = PRType_PRString;
    }
    return ret;
}

Object *
Component::GetObjectByGUUID(const string& guuid)
{
    return  m_objectsByGUUID[guuid];
}

Object *
Component::GetObjectByJSObject(JSObject *obj)
{
    list <Object *>::iterator iter;
    Object *pObj, *ret = (Object *) NULL;

    for (iter = m_objects.begin(); iter != m_objects.end(); ++iter) {
        pObj = *iter;
        if (obj == pObj->GetJSObject()) {
            ret = pObj;
            break;
        }
    }
    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
GetObject(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{   
    JSBool ret = JS_TRUE;
    JSString *str;

    if (!argc)
        return JS_TRUE;

    str = JS_ValueToString(cx, argv[0]);
    if (!str)
        return JS_FALSE;

    char *bytes = JS_GetStringBytes(str);
    bytes = strdup(bytes);

    // argv[0] has the GUUID of the component
    // find the component associated with the passed in object

    ComponentManager *mgr = ComponentManager::GetInstance();

    Component *comp = mgr->GetComponentByJSObject(obj);

    if (comp != (Component *) NULL) {

        // make sure the component library is loaded in memory

        bool isLoaded = comp->GetIsLoaded();
        if (!isLoaded) 
            ret = JS_FALSE;
        else {

            // find the class/object identified by the passed in GUUID

            Object *compObj = comp->GetObjectByGUUID(string(bytes));
            if (compObj) {

                // now that we have identified the class within the
                // component, create actual objects. 

                JSObject *newObj;

                compObj->CreateJSObject();
                newObj = compObj->GetJSObject();
                if (newObj) {
                    *rval = OBJECT_TO_JSVAL(newObj);
                } else {
                    ret = JS_FALSE;
                }
            }
        }
    } else {
        ret = JS_FALSE;
    }

    //fputs(bytes, stderr);
    free(bytes);
    return ret;
}

JS_STATIC_DLL_CALLBACK(JSBool)
ReleaseObject(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{   
    JSBool ret = JS_TRUE;

    if (!argc)
        return JS_TRUE;

    JSObject *jsObj;
    if (JS_ValueToObject(cx, argv[0], &jsObj) == JS_FALSE || !jsObj)
        return JS_FALSE;

    // find the component associated with the passed in object

    ComponentManager *mgr = ComponentManager::GetInstance();

    Component *comp = mgr->GetComponentByJSObject(obj);

    if (comp != (Component *) NULL) {

        // find the object that wraps this JS object

        Object *aObject;

        aObject = comp->GetObjectByJSObject(jsObj);

        if (aObject) 
            aObject->CallDestroyer();
        else
            ret = JS_FALSE;
    }

    return ret;

#if 0
    ComponentManager *mgr = ComponentManager::GetInstance();

    Component *comp = mgr->GetComponentByJSObject(obj);

    if (comp != (Component *) NULL) {

        // make sure the component library is loaded in memory

        bool isLoaded = comp->GetIsLoaded();
        if (!isLoaded) 
            ret = JS_FALSE;
        else {

            // find the class/object identified by the passed in GUUID

            Object *compObj = comp->GetObjectByGUUID(string(bytes));
            if (compObj) {

                // now that we have identified the class within the
                // component, create actual objects. 

                JSObject *newObj;

                compObj->CreateJSObject();
                newObj = compObj->GetJSObject();
                if (newObj) {
                    *rval = OBJECT_TO_JSVAL(newObj);
                } else {
                    ret = JS_FALSE;
                }
            }
        }
    } else {
        ret = JS_FALSE;
    }

    //fputs(bytes, stderr);
    free(bytes);

    return ret;
#endif
}

static JSFunctionSpec component_functions[] = {
    {"getObject", GetObject,  1, 0, 0},
    {"releaseObject", ReleaseObject,  1, 0, 0},
    {0, 0, 0, 0, 0}
};

PRStatus
Component::CreateJSObject()
{
    JSEngine *js = JSEngine::GetJSEngine();

    if (!js)
        return PR_FAILURE;

    JSContext *ctx = js->GetGlobalContext();
    JSObject *obj = js->GetGlobalObject(); // XXX global object ok?

/*
    JSClass component_class = {
        //"component", JSCLASS_NEW_RESOLVE,
        "component", 0,
        JS_PropertyStub,  JS_PropertyStub,
        JS_PropertyStub,  JS_PropertyStub,
        JS_EnumerateStub,  JS_ResolveStub,
        JS_ConvertStub,   JS_FinalizeStub
    };
*/

    JSAutoRequest r(ctx);
    m_jsObj = JS_DefineObject(ctx, obj, "component", &m_jsClass, NULL, 0);

    JS_DefineFunctions(ctx, m_jsObj, component_functions);

    return PR_SUCCESS;
}

JSObject *
Component::GetJSObject()
{
    return m_jsObj;
}

PRStatus
Object::SetComponent(Component *component)
{
    m_component = component;
    return PR_SUCCESS;
}

PRStatus
Object::CallDestroyer()
{
    (*m_pDestroyer)(m_jsObj);
    return PR_SUCCESS;
}

Component *
Object::GetComponent()
{
    return m_component;
}

PRStatus
Object::CreateJSObject()
{
    PRStatus ret = PR_SUCCESS;
    JSEngine *js = JSEngine::GetJSEngine();

    if (!js)
        return PR_FAILURE;

    Component *comp = GetComponent();

    if (!comp)
        return PR_FAILURE;

    Library *lib = comp->GetLibrary();

    if (!lib) 
        return PR_FAILURE;

    JSContext *ctx = js->GetGlobalContext();
    JSObject *obj = js->GetGlobalObject();

    string name = GetAttributeByName(string("name"))->GetValue();

    // create an instance of the proxy class

    string creator, destroyer, lookup;

    creator = "New" + name + "Proxy";
    destroyer = "Delete" + name + "Proxy";

    CreatorFn pCreator;

    pCreator = (CreatorFn) lib->FindSymbol(creator);
    m_pDestroyer = (DestroyerFn) lib->FindSymbol(destroyer);

    if (!pCreator || !m_pDestroyer) 
        return PR_FAILURE;

    m_name = name;

    JSClass objectClass = {
        //name.c_str(), JSCLASS_NEW_RESOLVE,
        m_name.c_str(), 0,
        JS_PropertyStub,  JS_PropertyStub,
        JS_PropertyStub,  JS_PropertyStub,
        JS_EnumerateStub,  JS_ResolveStub,
        JS_ConvertStub,   JS_FinalizeStub
    };        

    m_jsClass = objectClass;
   
    JSAutoRequest r(ctx); 
    m_jsObj = JS_DefineObject(ctx, obj, name.c_str(), 
        &m_jsClass, NULL, 0);

    if (!m_jsObj) 
        return PR_FAILURE;

    ret = (*pCreator)(m_jsObj);

    if (ret != PR_SUCCESS) {
        m_jsObj = NULL;
        return PR_FAILURE; // XXX destroy m_jsObj 
    }

    // iterate the functions defined by this object

    list <Function *> funcs = GetFunctionList();
    list <Function *>::iterator funciter; 
    for (funciter = funcs.begin(); funciter != funcs.end(); ++funciter) {
        Function *func = *funciter;
          
        AnAttribute *nameattr;
            
        nameattr = func->GetAttributeByName("name");
        string fnName = nameattr->GetValue();

        string name2 = name + "_" + fnName;
    
        list <Arg *> args = func->GetArgumentList();
        int size = args.size();

        // query the component for the proxy lib entry point

        JSNative fptr = (JSNative) lib->FindSymbol(name2); 

        if (fptr) {
            JSFunction *func = JS_DefineFunction(ctx, m_jsObj, fnName.c_str(),
                fptr,  // JSNative
                size, 0); 
            if (!func) {
                ret = PR_FAILURE;
                // XXX destroy m_jsObj, leak
                m_jsObj = NULL;
                break;
            } 
        } else {
            ret = PR_FAILURE;
            // XXX destroy m_jsObj, leak
            m_jsObj = NULL;
            break;
        }
    }

    return ret;
}       

JSObject *
Object::GetJSObject()
{           
    return m_jsObj;
}

string& Object::GetGUUID()
{
    AnAttribute *attribute;
    static string failret("");

    attribute = GetAttributeByName(string("id"));
    if (attribute)
        return attribute->GetValue();
    return failret;
}

