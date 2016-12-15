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

#include "windowswindowimpl.h"
#include "windowssaveaspickerimpl.h"

WindowsSaveAsPickerImpl::WindowsSaveAsPickerImpl() : m_saveaspicker(NULL)
{
}

WindowsSaveAsPickerImpl::~WindowsSaveAsPickerImpl()
{
}

PRStatus WindowsSaveAsPickerImpl::Create(const string &title)
{
    m_saveaspicker = __gc new SaveFileDialog(); 
    if (m_saveaspicker) {
        m_saveaspicker->Title = title.c_str();
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus WindowsSaveAsPickerImpl::GetFile(const string &path, string &files)
{
    files = "";
    if (m_saveaspicker && m_saveaspicker->ShowDialog() == DialogResult::OK) {
        String *filesret[] = m_saveaspicker->FileNames;
        bool first = true;
        for (int i = 0; i < m_saveaspicker->FileNames->Length; i++) {
            int len = filesret[i]->Length;
            if (len) {
                if (!first)
                    files += ",";
                first = false;
                char *buf;
                buf = reinterpret_cast<char *>(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filesret[i]).ToPointer());
                files += buf;
            } 
        }
    }
    return PR_SUCCESS;
}

