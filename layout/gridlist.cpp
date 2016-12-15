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

#include "gridlist.h"

GridList::GridList() 
{
	WidgetFactory *factory = GetWidgetFactory();

	if (factory)
		m_gridlist = factory->MakeGridList();
}

GridList::~GridList()
{
    if (m_gridlist)
        delete m_gridlist;
}

PRStatus GridList::GetValue(XPVariant &v)
{
    return PR_FAILURE;
}

PRStatus GridList::SetValue(const XPVariant &v)
{
    return PR_FAILURE;
}

PRStatus GridList::Draw()
{
	return PR_SUCCESS;
}

PRStatus GridList::Show()
{
    if (m_gridlist)
	    m_gridlist->Show();
    return PR_SUCCESS;
}

PRStatus GridList::Hide()
{
    if (m_gridlist)
	    m_gridlist->Hide();
    return PR_SUCCESS;
}

PRStatus GridList::Enable()
{
    if (m_gridlist)
	    m_gridlist->Enable();
    SetEnabled(true);
    return PR_SUCCESS;
}

PRStatus GridList::Disable()
{
    if (m_gridlist)
	    m_gridlist->Disable();
    SetEnabled(false);
    return PR_SUCCESS;
}

PRStatus GridList::Create()
{    
    PRStatus status;

    status = m_gridlist->Create();
    return status;
}

PRStatus GridList::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_gridlist)
        m_gridlist->GetGeometry(x, y, width, height);
    return PR_SUCCESS;
}

PRStatus GridList::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char& mask)
{
    if (m_gridlist)
        m_gridlist->SetGeometry(x, y, width, height, mask);
    return PR_SUCCESS;
}

PRStatus GridList::GetSelectionCount(int &count)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) 
        ret = m_gridlist->GetSelectionCount(count);
    return ret;
}

PRStatus GridList::GetSelection(const int index, string &value)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) 
        ret = m_gridlist->GetSelection(index, value);
    return ret;
}

PRStatus GridList::RemoveItemByValue(const string &value)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) 
        ret = m_gridlist->RemoveItemByValue(value);
    return ret;
}

PRStatus GridList::RemoveItemByPosition(const int position)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) 
        ret = m_gridlist->RemoveItemByPosition(position);
    return ret;
}

PRStatus GridList::AddItem(const string &value, const int position)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) 
        ret = m_gridlist->AddItem(value, position);
    return ret;
}

PRStatus GridList::AppendItem(const string &value)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) 
        ret = m_gridlist->AppendItem(value);
    return ret;
}
