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

#include "variant.h"

XPVariant::XPVariant() : m_type(XPVarTypeNone)
{
}

PRStatus XPVariant::SetValue(const char &val) 
{
    m_val.vChar = val; 
    m_type = XPVarTypeChar; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(char &val) 
{
    if (m_type == XPVarTypeChar) {
        val = m_val.vChar; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const PRUint8 &val) 
{
    m_val.vUint8 = val; 
    m_type = XPVarTypeUint8; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(PRUint8 &val) 
{
    if (m_type == XPVarTypeUint8) {
        val = m_val.vUint8; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const PRInt8 &val) 
{
    m_val.vInt8 = val; 
    m_type = XPVarTypeInt8; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(PRInt8 &val) 
{
    if (m_type == XPVarTypeInt8) {
        val = m_val.vInt8; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const PRUint16 &val) 
{
    m_val.vUint16 = val; 
    m_type = XPVarTypeInt16; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(PRUint16 &val) 
{
    if (m_type == XPVarTypeUint16) {
        val = m_val.vUint16; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const PRInt16 &val) 
{
    m_val.vInt16 = val; 
    m_type = XPVarTypeInt16; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(PRInt16 &val) 
{
    if (m_type == XPVarTypeInt16) {
        val = m_val.vInt16; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const PRUint32 &val) 
{
    m_val.vUint32 = val; 
    m_type = XPVarTypeUint32; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(PRUint32 &val) 
{
    if (m_type == XPVarTypeUint32) {
        val = m_val.vUint32; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const PRInt32 &val) 
{
    m_val.vInt32 = val; 
    m_type = XPVarTypeInt32; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(PRInt32 &val) 
{
    if (m_type == XPVarTypeInt32) {
        val = m_val.vInt32; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

#if defined(HAVE_LONG_LONG)
PRStatus XPVariant::SetValue(const PRUint64 &val) 
{
    m_val.vUint64 = val; 
    m_type = XPVarTypeUint64; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(PRUint64 &val) 
{
    if (m_type == XPVarTypeUint64) {
        val = m_val.vUint64; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const PRInt64 &val) 
{
    m_val.vInt64 = val; 
    m_type = XPVarTypeInt64; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(PRInt64 &val) 
{
    if (m_type == XPVarTypeInt64) {
        val = m_val.vInt64; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}
#endif // HAVE_LONG_LONG

PRStatus XPVariant::SetValue(const PRFloat64 &val) 
{
    m_val.vFloat64 = val; 
    m_type = XPVarTypeFloat64; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(PRFloat64 &val) 
{
    if (m_type == XPVarTypeFloat64) {
        val = m_val.vFloat64; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const bool &val) 
{
    m_val.vBool = val; 
    m_type = XPVarTypeBool; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(bool &val) 
{
    if (m_type == XPVarTypeBool) {
        val = m_val.vBool; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const string &val) 
{
    m_vString = val; 
    m_type = XPVarTypeString; 

    return PR_SUCCESS;
}

PRStatus XPVariant::GetValue(string &val) 
{
    if (m_type == XPVarTypeString) {
        val = m_vString; 
        return PR_SUCCESS; 
    } else 
        return PR_FAILURE; 
}

PRStatus XPVariant::SetValue(const char *val) 
{
    m_vString = val; 
    m_type = XPVarTypeString; 

    return PR_SUCCESS;
}

