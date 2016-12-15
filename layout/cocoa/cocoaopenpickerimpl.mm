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

#include "cocoawindowimpl.h"
#include "cocoaopenpickerimpl.h"

CocoaOpenPickerImpl::CocoaOpenPickerImpl() : m_openpicker(NULL)
{
}

CocoaOpenPickerImpl::~CocoaOpenPickerImpl()
{
}

PRStatus CocoaOpenPickerImpl::Create(const string &title)
{
    m_openpicker = [NSOpenPanel openPanel];
    if (m_openpicker) 
        return PR_SUCCESS;
	return PR_FAILURE;
}

PRStatus CocoaOpenPickerImpl::GetFile(const string &path, string &files)
{
    files = "";

    // Set the options for how the get file dialog will appear

    //SetDialogTitle(inTitle, m_openpicker);
    [m_openpicker setAllowsMultipleSelection:YES];
    [m_openpicker setCanSelectHiddenExtension:YES];
    [m_openpicker setCanChooseDirectories:NO];
    [m_openpicker setCanChooseFiles:YES];
    [m_openpicker setResolvesAliases:YES];

    // set up default directory

    int result = [m_openpicker runModalForDirectory:nil file:nil types:nil]; 
  
    if (result != NSFileHandlingPanelCancelButton) {

        // append each chosen file to our list, creating a CSV

        bool first = true;
        for (unsigned int i = 0; i < [[m_openpicker filenames] count]; i ++) {
            NSString *path = [[m_openpicker filenames] objectAtIndex:i];
            if (path) {
                if (!first)
                    files += ",";
                first = false;
                char *buffer;
                buffer = (char *) malloc([path length] + 1);
                if (buffer) {
                    [path getCString: buffer];
                    files += buffer;
                    free(buffer);
                } 
            }
        }
    }

    return PR_SUCCESS;
}

#if 0
PRStatus CocoaOpenPickerImpl::HandleCommand()
{
    NotifyButtonPress();
    return PR_SUCCESS;
}
#endif

