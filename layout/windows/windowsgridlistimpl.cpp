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

#include "windowsgridlistimpl.h"

WindowsGridListImpl::WindowsGridListImpl() : m_gridlist(NULL)
{
}

WindowsGridListImpl::~WindowsGridListImpl()
{
}

PRStatus WindowsGridListImpl::Create()
{
    m_gridlist = __gc new ListBox();
    if (m_gridlist) {
        m_gridlist->SelectionMode = SelectionMode::MultiExtended;
        if (!m_formParent) {
            WidgetImpl *top = GetRootWidget();
            if (top) {
                SetWidgetParent(top);
            }
        }

        bool added = false;
        if (m_formParent) {
            m_formParent->Controls->Add(m_gridlist);
            added = true;
        } else if (m_svParent) {
            m_svParent->Controls->Add(m_gridlist);
            added = true;
        }
        if (added) {
/*
            GridListCallbackHelper *pHelper;
            pHelper = m_gridlistHelper = __gc new GridListCallbackHelper(this);
            m_gridlist->add_Click(new
                EventHandler(pHelper,&GridListCallbackHelper::OnGridListClick));
*/
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus
WindowsGridListImpl::Show()
{
    if (m_gridlist) {
        m_gridlist->Show();
        return PR_SUCCESS;
    }
    return PR_FAILURE; 
}

PRStatus
WindowsGridListImpl::Hide()
{
    if (m_gridlist) {
        m_gridlist->Hide();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsGridListImpl::Enable()
{
    if (m_gridlist) {
        m_gridlist->Enabled = true;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus
WindowsGridListImpl::Disable()
{
    if (m_gridlist) {
        m_gridlist->Enabled = false;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus 
WindowsGridListImpl::SetGeometry(const int &x, const int &y,
    const int &width, const int &height, const char &mask)
{
    if (m_gridlist) {
        Size size;
        Point point;

        size = m_gridlist->ClientSize;
        point = m_gridlist->Location;

        if (mask & GEOM_X)
            point.X = x;
        if (mask & GEOM_Y)
            point.Y = y;
        if (mask & GEOM_WIDTH)
            size.Width = width;
        if (mask & GEOM_HEIGHT)
            size.Height = height;

        m_gridlist->ClientSize=size;
        m_gridlist->Location=point;
    } 

    return PR_SUCCESS;
}

PRStatus 
WindowsGridListImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    Size size, request(0, 0);
    
    if (m_gridlist) {
        size = m_gridlist->ClientSize;
        width = size.get_Width();
        height = size.get_Height();
    }
    return PR_SUCCESS;
}

PRStatus WindowsGridListImpl::GetSelectionCount(int &count)
{
    if (m_gridlist) {
        count = m_gridlist->SelectedIndices->Count;
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus WindowsGridListImpl::GetSelection(const int index, string &value)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) {
        int count;
        GetSelectionCount(count);
        if (count && index <= count) {
            String __gc *str = static_cast<String __gc *>(m_gridlist->SelectedItems->Item[index]);
            char *buf;
            buf = reinterpret_cast<char *>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str).ToPointer());
            value = buf;
            ret = PR_SUCCESS;
        }
    }
    return ret;
}

PRStatus WindowsGridListImpl::RemoveItemByValue(const string &value)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist && m_gridlist->Items->Count > 0) {
        Clear();
        String *str = value.c_str();
        int index = m_gridlist->Items->IndexOf(str);
        if (index != -1) {
            m_gridlist->BeginUpdate();
            m_gridlist->Items->RemoveAt(index);
            m_gridlist->EndUpdate();
            ret = PR_SUCCESS;
        }
    }
    return ret;
}

PRStatus WindowsGridListImpl::RemoveItemByPosition(const int position)
{
    PRStatus ret = PR_FAILURE;
    int count;
    if (m_gridlist && (count = m_gridlist->Items->Count) > 0) {
        if (position <= count) {
            Clear();
            m_gridlist->BeginUpdate();
            m_gridlist->Items->RemoveAt(position - 1);
            m_gridlist->EndUpdate();
            ret = PR_SUCCESS;
        }
    }
    return ret;
}

PRStatus WindowsGridListImpl::AddItem(const string &value, const int position)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) {
        Clear();
        m_gridlist->BeginUpdate();
        String *str = value.c_str();
        m_gridlist->Items->Insert(position, str);
        m_gridlist->EndUpdate();
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus WindowsGridListImpl::AppendItem(const string &value)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) {
        m_gridlist->BeginUpdate();
        String *str = value.c_str();
        m_gridlist->Items->Add(str);
        m_gridlist->EndUpdate();
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus WindowsGridListImpl::Clear()
{
    if (m_gridlist) 
        m_gridlist->ClearSelected();
    return PR_SUCCESS;
}

