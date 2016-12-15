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

#if !defined(__VARIANT_H__)
#define __VARIANT_H__

#include "prtypes.h"
#include <string>

using namespace std;

class XPVariant 
{
public:
	XPVariant();
	virtual ~XPVariant() {};

    typedef enum {
        XPVarTypeNone,
        XPVarTypeChar,
        XPVarTypeUint8,
        XPVarTypeInt8,
        XPVarTypeUint16,
        XPVarTypeInt16,
        XPVarTypeUint32,
        XPVarTypeInt32,
#if defined(HAVE_LONG_LONG)
        XPVarTypeUint64,
        XPVarTypeInt64,
#endif
        XPVarTypeFloat64,
        XPVarTypeBool,
        XPVarTypeString
    } XPVarType;

    XPVarType GetType() { return m_type; };

    // setters and getters

    PRStatus SetValue(const char &val);
    PRStatus GetValue(char &val);

    PRStatus SetValue(const PRUint8 &val);
    PRStatus GetValue(PRUint8 &val); 
    PRStatus SetValue(const PRInt8 &val); 
    PRStatus GetValue(PRInt8 &val); 
    PRStatus SetValue(const PRUint16 &val); 
    PRStatus GetValue(PRUint16 &val); 
    PRStatus SetValue(const PRInt16 &val); 
    PRStatus GetValue(PRInt16 &val); 
    PRStatus SetValue(const PRUint32 &val); 
    PRStatus GetValue(PRUint32 &val); 
    PRStatus SetValue(const PRInt32 &val); 
    PRStatus GetValue(PRInt32 &val); 
#if defined(HAVE_LONG_LONG)
    PRStatus SetValue(const PRUint64 &val); 
    PRStatus GetValue(PRUint64 &val); 
    PRStatus SetValue(const PRInt64 &val); 
    PRStatus GetValue(PRInt64 &val); 
#endif
    PRStatus SetValue(const PRFloat64 &val); 
    PRStatus GetValue(PRFloat64 &val); 

    PRStatus SetValue(const bool &val); 
    PRStatus GetValue(bool &val); 

    PRStatus SetValue(const char *val); 
    PRStatus SetValue(const string &val); 
    PRStatus GetValue(string &val); 

private:
    XPVarType m_type;
    union {
        char vChar;
        PRUint8 vUint8;
        PRInt8 vInt8;
        PRUint16 vUint16;
        PRInt16 vInt16; 
        PRUint32 vUint32;
        PRInt32 vInt32; 
#ifdef HAVE_LONG_LONG
        PRUint64 vUint64;
        PRInt64 vInt64; 
#endif
        PRFloat64 vFloat64;
        PRBool vBool;
    } m_val;
    string m_vString;
};

#endif
