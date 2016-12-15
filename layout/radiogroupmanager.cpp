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

#include "radiogroupmanager.h"

RadioGroupManager::RadioGroupManager()
{
    m_groups.clear();
}

int
RadioGroupManager::GetGroupSize(const string &group)
{
    int size = 0;
    list <RadioButtonImpl *> *rbList; 
    if (m_groups.find(group) != m_groups.end()) {
        rbList = m_groups[group];
        if (rbList)
            size = rbList->size();
    }
    return size;
}

RadioButtonImpl *
RadioGroupManager::GetFirstItem(const string &group)
{
    RadioButtonImpl *impl = NULL;
    if (m_groups.find(group) != m_groups.end()) {
        list <RadioButtonImpl *> *rbList; 
        rbList = m_groups[group];
        if (rbList) 
            impl = rbList->front();
    }
    return impl;
}

PRStatus 
RadioGroupManager::AddToGroup(const string &group, RadioButtonImpl *impl)
{
    list <RadioButtonImpl *> *rbList; 
    if (m_groups.find(group) == m_groups.end()) {
        rbList = new list<RadioButtonImpl *>();
        if (rbList) {
            m_groups[group] = rbList;
        }
    }
    rbList = m_groups[group];
    rbList->push_back(impl);
    return PR_SUCCESS;
}

PRStatus 
RadioGroupManager::RemoveFromGroup(const string &group, RadioButtonImpl *impl)
{
    if (m_groups.find(group) != m_groups.end()) {
        list <RadioButtonImpl *> *rbList; 
        rbList = m_groups[group];
        if (rbList) {
            rbList->remove(impl); 
        }
    }
    return PR_SUCCESS;
}

PRStatus 
RadioGroupManager::SetState(const string &group, RadioButtonImpl *impl)
{
    if (m_groups.find(group) != m_groups.end()) {
        list <RadioButtonImpl *> *rbList; 
        rbList = m_groups[group];
        if (rbList) {
            list <RadioButtonImpl *>::iterator iter;
            for (iter = rbList->begin(); iter != rbList->end(); ++iter) {
                if ((*iter) != impl) 
                    (*iter)->SetChecked(false);
            }
        }
    }
    return PR_SUCCESS;
}

PRStatus 
RadioGroupManager::AddManager(const Document *doc)
{
    InitDocumentMap();
    if (!doc)
        return PR_FAILURE;
    RadioGroupManager *mgr = new RadioGroupManager();
    if (!mgr)
        return PR_FAILURE;
    m_documentMap[doc] = mgr;
    return PR_SUCCESS; 
}

PRStatus 
RadioGroupManager::RemoveManager(const Document *doc)
{
    InitDocumentMap();
    if (!doc)
        return PR_FAILURE;
    map <const Document *, RadioGroupManager *>::iterator iter;
    if ((iter = m_documentMap.find(doc)) != m_documentMap.end()) {
        RadioGroupManager *mgr = m_documentMap[doc];
        if (mgr) 
            delete mgr;
        m_documentMap.erase(iter); 
    }
    return PR_SUCCESS; 
}

RadioGroupManager * 
RadioGroupManager::GetManager(const Document *doc)
{
    InitDocumentMap();
    if (!doc)
        return NULL;
    return m_documentMap[doc];
}

void
RadioGroupManager::InitDocumentMap()
{
    if (!m_documentMapInited) {
        m_documentMap.clear();
        m_documentMapInited = true;
    }
}

map <const Document *, RadioGroupManager *> RadioGroupManager::m_documentMap;
bool RadioGroupManager::m_documentMapInited = false;
