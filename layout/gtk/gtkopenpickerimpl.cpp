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

#include "gtkwindowimpl.h"
#include "gtkopenpickerimpl.h"

GtkOpenPickerImpl::GtkOpenPickerImpl() : m_openpicker(NULL)
{
}

GtkOpenPickerImpl::~GtkOpenPickerImpl()
{
}

extern "C" {

gint HandleOpenPickerOkThunk(GtkWidget *widget, gpointer callbackData)
{
    GtkOpenPickerImpl *pPickerImpl = (GtkOpenPickerImpl *) callbackData;
    if (pPickerImpl) {
        GtkWidget *w = pPickerImpl->GetImpl();
        if (w) {
            pPickerImpl->SetFilename(
                gtk_file_selection_get_filename(GTK_FILE_SELECTION(w)));
            pPickerImpl->SetOkPressed(true);
        }
        gtk_main_quit();
    }
    return TRUE;
}

gint HandleOpenPickerCancelThunk(GtkWidget *widget, gpointer callbackData)
{
    gtk_main_quit();
    return TRUE;
}

} // extern "C"

PRStatus GtkOpenPickerImpl::Create(const string &title)
{
    m_openpicker = gtk_file_selection_new(const_cast<char *>(title.c_str())); 
    if (m_openpicker) {
        SetFilename("");
        SetOkPressed(false);

        gtk_file_selection_hide_fileop_buttons(
            GTK_FILE_SELECTION(m_openpicker));

        gtk_signal_connect(GTK_OBJECT(
            GTK_FILE_SELECTION(m_openpicker)->ok_button),
            "clicked", GTK_SIGNAL_FUNC(HandleOpenPickerOkThunk), this);
               
        gtk_signal_connect_object(GTK_OBJECT(
            GTK_FILE_SELECTION(m_openpicker)->ok_button),
            "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
            GTK_OBJECT(m_openpicker));

        gtk_signal_connect(GTK_OBJECT(
            GTK_FILE_SELECTION(m_openpicker)->cancel_button),
            "clicked", GTK_SIGNAL_FUNC(HandleOpenPickerCancelThunk), this);
               
        gtk_signal_connect_object(GTK_OBJECT(
            GTK_FILE_SELECTION(m_openpicker)->cancel_button),
            "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
            GTK_OBJECT(m_openpicker));
   
        return PR_SUCCESS;
    }
	return PR_FAILURE;
}

PRStatus GtkOpenPickerImpl::GetFile(const string &path, string &files)
{
    files = "";
    if (m_openpicker) {
        gtk_widget_show(m_openpicker);
        gtk_main();
    }
    if (GetOkPressed() == true) {
        files = m_filename;
    }

#if 0
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
#endif
    return PR_SUCCESS;
}

