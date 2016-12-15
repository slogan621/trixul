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

#if !defined( __COMPONENT_MGR_H__ )
#define __COMPONENT_MGR_H__

#include <string>
#include <map>
#include "component.h"
#include <expat.h>
#include "prtypes.h"

#include "jsengine.h"

#include <fstream>

using namespace std;

class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();
    static ComponentManager* GetInstance();
    Component *GetComponentByJSObject(JSObject *obj);
    PRStatus ProcessComponents();
	Component *ProcessFile(const string& path, bool live);
	PRStatus ParseSIL(const string& path, const string& proxyHeaderOut,
        const string& proxySourceOut, const string& componentHeaderOut);
	PRStatus CreateJSObject();
	JSObject *GetJSObject();
    string& GetGUUID();
    PRStatus LoadComponent(Component *comp);
    Component *GetComponentByGUUID(const string& guuid);
private:
	map <string, Component *> m_components;
	XML_Parser m_parser; 
    JSObject *m_jsObj;
};
#endif
