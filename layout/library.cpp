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

#include "library.h"

#include "plbasename.h"

Library::Library()
{
}

Library::~Library()
{
    m_symbols.clear();
    Unload();
}

PRStatus Library::Load(const string& path)
{
    PRStatus ret = PR_FAILURE;

    string base(PL_basename(const_cast<char *>(path.c_str())));
    string dir(PL_dirname(const_cast<char *>(path.c_str())));
    char *libname;
    unsigned int size;

    size = base.find_last_of(".");

    if (size != string::npos) 
        base.resize(base.find_last_of("."));

    libname = PR_GetLibraryName(dir.c_str(), base.c_str());

    if (libname) {
        m_lib = PR_LoadLibrary(libname);
        if (m_lib)  {
            m_path = path;
            ret = PR_SUCCESS;
        }
    } 
    return ret;
}

typedef void *(*LookupFn)(void *, const char *);
PRFuncPtr Library::FindSymbol(const string& name)
{
    PRFuncPtr symbol = NULL;
    LookupFn foo;
    if (m_lib) {
        symbol = m_symbols[name];		
        if (symbol == NULL) {
            // find it, and add to map
            symbol = PR_FindFunctionSymbol(m_lib, name.c_str());
            if (symbol) 
                m_symbols[name] = symbol;
        }
    }
    foo = (LookupFn) symbol;
    return symbol;
}

string& Library::GetPath()
{
    return m_path;
}

PRStatus Library::Unload()
{
    PRStatus status = PR_SUCCESS;
    if (m_lib) {
        status = PR_UnloadLibrary(m_lib);
        m_lib = NULL;
    }
    return status;
}
