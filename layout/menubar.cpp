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

#include "menubar.h"
#include "menu.h"

Menubar::Menubar() 
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory) 
		m_menubar = factory->MakeMenubar();
}

Menubar::~Menubar()
{
    if (m_menubar)
        delete m_menubar;
}

PRStatus Menubar::Draw()
{
	return PR_SUCCESS;
}

PRStatus Menubar::Show()
{
    if (m_menubar)
	    m_menubar->Show();
    return PR_SUCCESS;
}

PRStatus Menubar::Hide()
{
    if (m_menubar)
	    m_menubar->Hide();
    return PR_SUCCESS;
}

PRStatus Menubar::AddMenu(Menu *menu)
{
    return PR_SUCCESS;
}

PRStatus Menubar::Create()
{    
    PRStatus status;

    status = m_menubar->Create();
    if (status == PR_SUCCESS) {
        m_menubar->PreCreate();
        status = CreateChildren();
        m_menubar->PostCreate();
    }
    return status;
}

PRStatus Menubar::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_menubar)
        m_menubar->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}

PRStatus Menubar::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char& mask)
{
    if (m_menubar)
        m_menubar->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus Menubar::ComputeLayout(const int &x, const int &y, const int &width,
    const int &height)
{
    if (m_menubar) {
       
        // take the entire available width

        int tx, ty, twidth, theight;

        m_menubar->GetGeometry(tx, ty, twidth, theight);
 
        SetGeometry(0, 0, width, theight, GEOM_ALL);
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus Menubar::GetGeometryRequestImpl(int &x, int &y, int &width, int &height)
{
    if (m_menubar)
        m_menubar->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}
