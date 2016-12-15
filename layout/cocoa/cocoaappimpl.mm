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

#include "cocoaappimpl.h"

CocoaAppImpl::CocoaAppImpl()
{
}

CocoaAppImpl::~CocoaAppImpl()
{
	[NSApp release];
	[m_pool release];
}

PRStatus CocoaAppImpl::Initialize(int *argc, char *argv[])
{
    m_pool = [[NSAutoreleasePool alloc]init];
    NSApp = [NSApplication sharedApplication];

    // This causes the main window and menubar to raise
    [NSApp activateIgnoringOtherApps: YES]; 

    return PR_SUCCESS;
}

PRStatus CocoaAppImpl::Shutdown()
{
    [NSApp stop: nil];

    // force an event into the event queue. Otherwise, NSApp won't
    // fall out of its loop. Alternative is for the user to wiggle
    // the mouse. Clearly, this is a bug in NSApplication of some kind.
    // see http://ftp2.tw.freebsd.org/pub/FreeBSD/distfiles/cs-pseudo-stable-2005-09-03.tar.bz2/CS/libs/csutil/macosx/ for acknowledgments.

    [NSApp postEvent: [NSEvent otherEventWithType:NSApplicationDefined
        location:NSMakePoint(0,0)
        modifierFlags:0
        timestamp:0
        windowNumber:0
        context:[NSApp context] subtype:0x3d8a data1:0x1827 data2:0x4851] 
        atStart:NO];
	return PR_SUCCESS;
}

PRStatus CocoaAppImpl::MainLoop()
{
   // in cocoa, we invoke the main loop this way

    [NSApp run];

    // when we get here, application has left its main loop

    return PR_SUCCESS;
}
