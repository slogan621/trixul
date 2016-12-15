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

#if !defined(__WIDGET_H__)
#define __WIDGET_H__

#include "element.h"
#include "prtypes.h"

#include "widgetimpl.h"
#include "boxlayout.h"

class Widget : public Element, public BoxLayout
{
public:
	Widget();
	virtual ~Widget() {};
	virtual PRStatus SetGeometry(const int &x, const int &y, 
        const int &width, const int &height, const char &mask) = 0;
	virtual PRStatus GetGeometry(int &x, int &y, int &width, int &height) = 0;
    virtual WidgetImpl *GetImpl() = 0;
	virtual PRStatus Create() = 0;
	virtual PRStatus Draw() = 0;
	virtual PRStatus Show() = 0;
	virtual PRStatus Hide() = 0;
	virtual ElementType GetType() = 0;
	virtual bool IsContainer() = 0;
	virtual bool IsSpacer() = 0;
    virtual char *GetTypeName() = 0;
    bool GetExplicitGeometry(char &mask, int &x, int &y, 
        int &width, int &height);
	PRStatus SetGeometryRequest(const int &x, const int &y, 
        const int &width, const int &height, const char &mask); 
	PRStatus GetGeometryRequest(int &x, int &y, int &width, int &height);
	PRStatus GetComputedGeometry(int &x, int &y, int &width, int &height);
	virtual PRStatus GetGeometryRequestImpl(int &x, int &y, 
        int &width, int &height) = 0;
    void SetOverrideExplicitGeometry(const bool override) {m_overrideExplicit = override;};
    bool GetOverrideExplicitGeometry() {return m_overrideExplicit;}; 
private:
    bool m_overrideExplicit;
    int m_explicitX, m_explicitY, m_explicitWidth, m_explicitHeight;
    int m_xRequest, m_yRequest, m_widthRequest, m_heightRequest;
};

#endif
