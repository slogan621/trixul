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
#include "gtksaveaspickerimpl.h"

GtkSaveAsPickerImpl::GtkSaveAsPickerImpl() : m_saveaspicker(NULL)
{
}

GtkSaveAsPickerImpl::~GtkSaveAsPickerImpl()
{
}

extern "C" {

gint HandleSaveAsPickerOkThunk(GtkWidget *widget, gpointer callbackData)
{
    GtkSaveAsPickerImpl *pPickerImpl = (GtkSaveAsPickerImpl *) callbackData;
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

gint HandleSaveAsPickerCancelThunk(GtkWidget *widget, gpointer callbackData)
{
    gtk_main_quit();
    return TRUE;
}

} // extern "C"

PRStatus GtkSaveAsPickerImpl::Create(const string &title)
{
    m_saveaspicker = gtk_file_selection_new(const_cast<char *>(title.c_str())); 
    SetFilename("");
    SetOkPressed(false);
    if (m_saveaspicker) {

        gtk_signal_connect(GTK_OBJECT(
            GTK_FILE_SELECTION(m_saveaspicker)->ok_button),
            "clicked", GTK_SIGNAL_FUNC(HandleSaveAsPickerOkThunk), this);

        gtk_signal_connect_object(GTK_OBJECT(
            GTK_FILE_SELECTION(m_saveaspicker)->ok_button),
            "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
            GTK_OBJECT(m_saveaspicker));

        gtk_signal_connect(GTK_OBJECT(
            GTK_FILE_SELECTION(m_saveaspicker)->cancel_button),
            "clicked", GTK_SIGNAL_FUNC(HandleSaveAsPickerCancelThunk), this);

        gtk_signal_connect_object(GTK_OBJECT(
            GTK_FILE_SELECTION(m_saveaspicker)->cancel_button),
            "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
            GTK_OBJECT(m_saveaspicker));

        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus GtkSaveAsPickerImpl::GetFile(const string &path, string &files)
{
    files = "";
    if (m_saveaspicker) {
        gtk_widget_show(m_saveaspicker);
        gtk_main();
    }
    if (GetOkPressed() == true) {
        files = m_filename;
    }
#if 0

    // Set the options for how the get file dialog will appear

    //SetDialogTitle(inTitle, m_saveaspicker);
    //[m_saveaspicker setAllowsMultipleSelection:YES];
    [m_saveaspicker setCanSelectHiddenExtension:YES];
    //[m_saveaspicker setCanChooseDirectories:NO];
    //[m_saveaspicker setCanChooseFiles:YES];
    //[m_saveaspicker setResolvesAliases:YES];

    // set up default directory

    int result = [m_saveaspicker runModalForDirectory:nil file:nil]; 
  
    if (result != NSFileHandlingPanelCancelButton) {

        // append each chosen file to our list, creating a CSV

        bool first = true;
        for (unsigned int i = 0; i < [[m_saveaspicker filenames] count]; i ++) {
            NSString *path = [[m_saveaspicker filenames] objectAtIndex:i];
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

