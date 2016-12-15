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

#if !defined(__CONTROL_H__)
#define __CONTROL_H__

#include "widget.h"
#include "variant.h"

typedef enum {
    ContentTypeNone,
    ContentTypeInt,
    ContentTypeBoolean,
    ContentTypeString
} ContentType;
    
class Control : public Widget
{
public:
    Control() : m_isEnabled(true), m_contentType(ContentTypeNone) {}
	virtual ~Control() {};
    virtual bool IsContainer() {return false;};
    virtual bool IsSpacer() {return false;};
    virtual PRStatus Disable() = 0;
    virtual PRStatus Enable() = 0;
    virtual PRStatus GetValue(XPVariant &v) = 0;
    virtual PRStatus SetValue(const XPVariant &v) = 0; 
    bool GetEnabled() {return m_isEnabled;};
    PRStatus SetEnabled(bool enabled) {m_isEnabled = enabled; return PR_SUCCESS;};
    virtual PRStatus GetSelectionCount(int &count) {return PR_FAILURE;};
    virtual PRStatus GetSelection(const int index, string &value) 
    {
        return PR_FAILURE;
    };
    virtual PRStatus RemoveItemByValue(const string &value) 
    {
        return PR_FAILURE;
    }
    virtual PRStatus RemoveItemByPosition(const int position) 
    {
        return PR_FAILURE;
    }
    virtual PRStatus AddItem(const string &value, const int position)
    {
        return PR_FAILURE;
    }
    virtual PRStatus AppendItem(const string &value)
    {
        return PR_FAILURE;
    }
    ContentType GetContentType() {return m_contentType;};
    PRStatus SetContentType(ContentType type) {m_contentType = type; 
        return PR_SUCCESS;};
    virtual void PrintInformation();   
private:
    bool m_isEnabled;
    ContentType m_contentType;
};

#endif
