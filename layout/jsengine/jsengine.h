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

#if !defined(__JSAPI_H__)
#define __JSAPI_H__

#include "jsapi.h"
#include "prtypes.h"
#include <stack>
#include <list>

#include "applistener.h"

using namespace std;

class JSEngine {
public:
    PRStatus InitEngine();
    PRStatus ReleaseEngine();
    JSContext *GetNewContext();
    void FreeContext(JSContext *ctx);
    void ReclaimContexts();
    PRStatus CreateContextPool();
    PRStatus FreeContextPool();
    JSObject *GetGlobalObject();
    JSContext *GetGlobalContext();
    JSObject *NewSuperGlobal(JSContext *ctx);
    static JSEngine *GetJSEngine();
    AppListener *GetApp() {return m_app;};
    void SetApp(AppListener *app) {m_app = app;};
private:
    static const int m_numContexts = 100;
    JSEngine() : m_app(NULL), m_rt(NULL), m_cx(NULL), m_glob(NULL) {};
    JSEngine(const JSEngine &);
    JSEngine &operator=(const JSEngine &);
    AppListener *m_app;
    JSRuntime *m_rt;
    JSContext *m_cx;
    JSObject *m_glob;
    list <JSContext *> m_recycle; // list of recycleable contexts
    list <JSContext *> m_pool; // pool of available contexts
};

#endif
