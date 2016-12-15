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

#include "script.h"

#include "layout.h"
#include "jsengine.h"

#include "plgetcwd.h"

Script::Script() : m_ctx(NULL), m_obj(NULL), m_script(NULL)
{
}

PRStatus
Script::Parse(JSObject *obj, JSContext *cx)
{
    if (!m_ctx)
        m_ctx = cx;

    if (m_ctx) {
        m_obj = obj;

        if (m_script) {
            JS_BeginRequest(m_ctx);
            JS_DestroyScript(m_ctx, m_script);
            JS_EndRequest(m_ctx);
            m_script = NULL;
        }

        if (m_obj) {

            // retrieve the src attribute

            AnAttribute *attribute;
            attribute = GetAttributeByName(string("src"));
            if (attribute) {
                string src = attribute->GetValue();

                // parse the src 
                // XXX only files right now, need to detect if it is
                // actual source vs a filename embedded in the src tag
                // if source, use JS_CompileScript()

                char *cwd = PL_getcwd(NULL, 0);
                if (cwd) {
                    string path(cwd);
                    free(cwd);

                    path += "/";
                    path += src.c_str();

                    JS_BeginRequest(m_ctx);

                    m_script = JS_CompileFile(m_ctx, m_obj, path.c_str());
                    if (m_script) {
                        JS_AddRoot(m_ctx, &m_script);

                        JSBool ret;
                        jsval rval;
                        ret = JS_ExecuteScript(m_ctx, m_obj, m_script, &rval);
                        JS_RemoveRoot(m_ctx, &m_script);
                    }
                    JS_EndRequest(m_ctx);
                }
            }
        }
    }

    return PR_SUCCESS;   
}

// called when the document is closed

PRStatus 
Script::Close()
{
    JSAutoRequest r(m_ctx);
    if (m_ctx && m_script) {
        JS_DestroyScript(m_ctx, m_script);
        m_script = NULL;
    }
    return PR_SUCCESS;
}
