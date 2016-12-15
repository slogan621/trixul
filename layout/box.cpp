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

#include "box.h"
#include "layout.h"
#include "document.h"
#include "spacer.h"

Box::Box() : m_x(0), m_y(0), m_width(0), m_height(0)
{
}

Box::~Box()
{
}

PRStatus Box::Draw()
{
	return PR_SUCCESS;
}

PRStatus Box::Create()
{
    PRStatus status;

    status = CreateChildren();
    return status;
}

PRStatus Box::Show()
{
    PRStatus status;

    status = ShowChildren();
    return status;
}

PRStatus Box::Hide()
{
	return HideChildren();
}

PRStatus Box::ComputeLayout(const int &x, const int &y, const int &width,
        const int &height)
{
#if 0
    return ComputeBoxLayout(x, y, width, height);
#else
    return PR_SUCCESS;
#endif
}

PRStatus Box::GetGeometry(int &x, int &y, int &width, int &height)
{
    x = m_x;
    y = m_y;
    width = m_width;
    height = m_height;

    return PR_SUCCESS;
}

PRStatus Box::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char &mask)
{
    if (mask & GEOM_X)
        m_x = x;
    if (mask & GEOM_Y)
        m_y = y;
    if (mask & GEOM_WIDTH)
        m_width = width;
    if (mask & GEOM_HEIGHT)
        m_height = height;
    return PR_SUCCESS;
}

