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

#include "scrolledwindow.h"
#include "scrolledview.h"

ScrolledView::ScrolledView() 
{
    SetOrientation(Vertical);
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_scrolledView = factory->MakeScrolledView();
}

ScrolledView::~ScrolledView()
{
}

PRStatus ScrolledView::Draw()
{
	return PR_SUCCESS;
}

PRStatus ScrolledView::Attach(ScrolledWindowImpl *scrolledWin)
{
    if (m_scrolledView)
        return m_scrolledView->Attach(scrolledWin);
    return PR_FAILURE;
}

PRStatus ScrolledView::ComputeLayout(const int &x, const int &y, 
    const int &width, const int &height)
{
    return ComputeBoxLayout(x, y, width, height);
}

PRStatus ScrolledView::Show()
{
    PRStatus status = PR_FAILURE;
    if (m_scrolledView) {
	    status = m_scrolledView->Show();
        if (status == PR_SUCCESS)
            status = ShowChildren();
    }
    return status;
}

PRStatus ScrolledView::Hide()
{
    PRStatus status = PR_FAILURE;
  
    if (m_scrolledView) {
        status = m_scrolledView->Hide();
        if (status == PR_SUCCESS)
            status = HideChildren();
    }
    return status;
}

PRStatus ScrolledView::Create()
{    
    PRStatus status; 
    status = m_scrolledView->Create();
    if (status == PR_SUCCESS)
        status = CreateChildren();
    return status;
}

PRStatus ScrolledView::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char& mask)
{
    if (m_scrolledView)
        m_scrolledView->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus ScrolledView::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_scrolledView)
        m_scrolledView->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}


