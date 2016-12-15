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

#include "widget.h"

Widget::Widget() : m_overrideExplicit(false),
    m_explicitX(0), m_explicitY(0), m_explicitWidth(0), m_explicitHeight(0),
    m_xRequest(0), m_yRequest(0), m_widthRequest(0), m_heightRequest(0)
{
}

PRStatus Widget::Draw()
{
	return PR_SUCCESS;
}

PRStatus Widget::Show()
{
	return PR_SUCCESS;
}

PRStatus Widget::Hide()
{
	return PR_SUCCESS;
}

bool 
Widget::GetExplicitGeometry(char &mask, int &x, int &y, int &width, int &height)
{
    mask = 0x00;
    AnAttribute *attribute;
    
    attribute = GetAttributeByName(string("x"));
    if (attribute) {
        mask |= GEOM_X; 
        x = attribute->GetValueAsInt();
    }               
            
    attribute = GetAttributeByName(string("y"));
    if (attribute) {
        mask |= GEOM_Y;
        y = attribute->GetValueAsInt();
    }           
            
    attribute = GetAttributeByName(string("width"));
    if (attribute) {
        mask |= GEOM_WIDTH;
        width = attribute->GetValueAsInt();
    }               

    attribute = GetAttributeByName(string("height"));
    if (attribute) {
        mask |= GEOM_HEIGHT;
        height = attribute->GetValueAsInt();
    }
  
    if (mask != 0x00)
        return true;
    return false;
}

PRStatus Widget::SetGeometryRequest(const int &x, const int &y,        
    const int &width, const int &height, const char &mask)
{
    if (mask & GEOM_X)
        m_xRequest = x;
    if (mask & GEOM_Y)
        m_yRequest = y;
    if (mask & GEOM_WIDTH)
        m_widthRequest = width;
    if (mask & GEOM_HEIGHT)
        m_heightRequest = height;
    return PR_SUCCESS;
}

PRStatus Widget::GetComputedGeometry(int &x, int &y, int &width, int &height)
{
    x = m_xRequest;
    y = m_yRequest;
    width = m_widthRequest;
    height = m_heightRequest;
    return PR_SUCCESS;
}

PRStatus Widget::GetGeometryRequest(int &x, int &y, int &width, int &height)
{
    GetComputedGeometry(x, y, width, height);

    // let the implementation change any of these it wishes

    GetGeometryRequestImpl(x, y, width, height);

    char mask = 0;
    int exX, exY, exW, exH;

    if (!GetOverrideExplicitGeometry())
        GetExplicitGeometry(mask, exX, exY, exW, exH);

    if (mask & GEOM_X)
        x = exX;
    if (mask & GEOM_Y)
        y = exY;
    if (mask & GEOM_WIDTH)
        width = exW;
    if (mask & GEOM_HEIGHT)
        height = exH;

    return PR_SUCCESS;
}

