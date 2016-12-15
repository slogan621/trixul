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

componentmgr.getComponent("94981D9E-FC99-11D9-8BDE-F66BAD1E3F3A");

var globalFoo = "Fernando Sor";

function Button1Click()
{
    dump("Inside of simple.js:Button1Click\n");
    dump("\n\n\n\nglobalFoo is " + globalFoo + "\n\n\n\n\n");
    globalFoo = "Amadeus Mozart";
    var obj = document.getElementById("text1");
    if (obj) {
        obj.enabled = false;
        obj.setValue("Mozart");
        obj.value = "Amadeus " + obj.value;
    }
    obj = document.getElementById("menuitem1");
    if (obj) {
        dump("Found a menu item to disable!\n");
        obj.disable();
    }
    obj = document.getElementById("button1");
    if (obj) {
        obj.enabled = false;
    }
    dump(170 + "\n");

    var component = componentmgr.getComponent("94981D9E-FC99-11D9-8BDE-F66BAD1E3F3A");
    if (component) {
        dump("Button1Click got the component\n");
        var anobject = component.getObject("2ABDF00B-025E-11DA-BFFE-000A27942344");

        if (anobject) {
            try {
            dump("Button1Click got the object\n");
            anobject.Hello("Syd"); // a function that does exist
            anobject.Foobar();     // a function that doesn't exist
            } catch (ex) {
                dump(ex);
            }
            component.releaseObject(anobject);
        } else {
            dump("Button1Click failed to get the object\n");
        }
    }
    dump("Leaving simple.js:Button1Click\n");
    return 17;
}

function Button2Click()
{
    dump("Inside of simple.js:Button2Click\n");
    dump("\n\n\n\nglobalFoo is " + globalFoo + "\n\n\n\n\n");
    globalFoo = "J.S.Bach";
    var obj = document.getElementById("text1");
    if (obj) {
        obj.enable();
        dump("Value of text1 (getValue()) is " + obj.getValue() + "\n");
        dump("Value of text1 (.value) is " + obj.value + "\n");
        obj.value="J.S. Bach";
    }
    obj = document.getElementById("menuitem1");
    if (obj) {
        dump("Found a menu item to enable!\n");
        obj.enable();
    }
    obj = document.getElementById("button1");
    if (obj) {
        obj.enable();
    }
    document.openDialog("resources/content/dialog.xul");
    try {
        componentmgr.getComponent("32381D9E-FC99-11D9-8BDE-F66BAD1E3F3A");
    } catch(ex) {
        dump(ex);
    }
    dump("Leaving simple.js:Button2Click\n");
    dump(180);
    return 18;
}

function MenuFileOpen()
{
    var paths = filepicker.openPicker();
    dump("Inside of MenuFileOpen " + paths + "\n\n");
}

function MenuFileQuit()
{
    dump("Inside of MenuFileQuit\n\n");
}

function MenuFileSaveAs()
{
    var paths = filepicker.saveAsPicker();
    dump("Inside of MenuFileSaveAs '" + paths + "'\n\n");
}


