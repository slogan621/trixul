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

#include "attributelist.h"
#include "prprf.h"

AnAttribute *AttributeList::AddAttribute(const string& name, const int value)
{
	char buf[128];

	PR_snprintf(buf, sizeof(buf), "%d", value);
	string stValue(buf);
	return AddAttribute(name, stValue);
}

AnAttribute *AttributeList::AddAttribute(const string& name, const string& value)
{
	AnAttribute *attribute;

	attribute = GetAttributeByName(name);

	if (attribute == NULL) {
		attribute = new AnAttribute();
		if (attribute)
			m_attributes.push_back(attribute);
	}

	if (attribute) {
		attribute->SetName(name);
		attribute->SetValue(value);
	}
	return attribute;
}

AnAttribute *AttributeList::GetAttributeByName(const string& name)
{
    list<AnAttribute *>::iterator itr;

    for (itr = m_attributes.begin(); itr != m_attributes.end(); ++itr)
        if ((*itr)->GetName() == name)
            return *itr;
    return NULL;
}

