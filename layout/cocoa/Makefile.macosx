# 
# Trixul Cross-Platform GUI Toolkit
# 
# Copyright (c) 2006, Syd Logan
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
# 
# * Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice,
# * this list of conditions and the following disclaimer in the documentation 
# and/or other materials provided with the distribution.
# * Neither the name trixul nor the names of its contributors may be used to
# endorse or promote products derived from this software without specific prior
# written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 

top=../..
include $(top)/local.mk

includes=-I../../dist/include/nspr -I../ -I../jsengine -I../jsengine/include
libs=../../dist/macosx/debug/layout.app/Contents/MacOS

all: libcocoawidget.dylib

libcocoawidget.dylib: cocoafactory.o cocoabuttonimpl.o cocoawindowimpl.o \
cocoaappimpl.o cocoabuttonaction.o cocoatextimpl.o cocoastatictextimpl.o \
cocoawidgetimpl.o cocoaresizeaction.o windowresizesubject.o cocoamenuimpl.o \
buttonpresssubject.o cocoamenuitemimpl.o cocoamenuitemaction.o \
menuitemclicksubject.o cocoamenubarimpl.o widgetimpl.o cocoapasteboardaction.o \
cocoaopenpickerimpl.o cocoasaveaspickerimpl.o windowraisesubject.o \
cocoascrolledwindowimpl.o cocoascrolledviewimpl.o cocoagridlistimpl.o \
gridlistdatasource.o cocoaradiobuttonimpl.o cocoacheckboximpl.o \
radiogroupmanager.o 
	g++ -g -arch $(arch) -dynamiclib -compatibility_version 1 -current_version 1 \
-install_name @executable_path/libcocoawidget.dylib -fPIC -framework Cocoa \
-o libcocoawidget.dylib cocoafactory.o cocoabuttonimpl.o cocoawindowimpl.o \
cocoaappimpl.o cocoabuttonaction.o cocoatextimpl.o cocoastatictextimpl.o \
cocoawidgetimpl.o cocoaresizeaction.o windowresizesubject.o cocoamenuimpl.o \
buttonpresssubject.o cocoamenuitemimpl.o cocoamenuitemaction.o \
menuitemclicksubject.o cocoamenubarimpl.o widgetimpl.o cocoapasteboardaction.o \
cocoaopenpickerimpl.o cocoasaveaspickerimpl.o windowraisesubject.o \
cocoascrolledwindowimpl.o cocoascrolledviewimpl.o cocoagridlistimpl.o \
gridlistdatasource.o cocoaradiobuttonimpl.o cocoacheckboximpl.o \
radiogroupmanager.o 
	cp libcocoawidget.dylib $(libs) 

radiogroupmanager.o: ../radiogroupmanager.cpp ../radiogroupmanager.h
	g++ -g $(includes)  -fPIC -Wall -c ../radiogroupmanager.cpp

cocoafactory.o: cocoafactory.mm cocoafactory.h
	g++ -g $(includes)  -fPIC -Wall -c cocoafactory.mm

cocoawindowimpl.o: cocoawindowimpl.mm cocoawindowimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoawindowimpl.mm

cocoagridlistimpl.o: cocoagridlistimpl.mm cocoagridlistimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoagridlistimpl.mm

gridlistdatasource.o: gridlistdatasource.mm gridlistdatasource.h
	g++ -g $(includes)  -fPIC -Wall -c gridlistdatasource.mm

cocoascrolledwindowimpl.o: cocoascrolledwindowimpl.mm cocoascrolledwindowimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoascrolledwindowimpl.mm

cocoascrolledviewimpl.o: cocoascrolledviewimpl.mm cocoascrolledviewimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoascrolledviewimpl.mm

cocoaappimpl.o: cocoaappimpl.mm cocoaappimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoaappimpl.mm

widgetimpl.o: ../widgetimpl.cpp ../widgetimpl.h
	g++ -g $(includes)  -fPIC -Wall -c ../widgetimpl.cpp 

cocoawidgetimpl.o: cocoawidgetimpl.mm cocoawidgetimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoawidgetimpl.mm

cocoamenuimpl.o: cocoamenuimpl.mm cocoamenuimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoamenuimpl.mm

cocoamenuitemimpl.o: cocoamenuitemimpl.mm cocoamenuitemimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoamenuitemimpl.mm

cocoamenubarimpl.o: cocoamenubarimpl.mm cocoamenubarimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoamenubarimpl.mm

cocoamenuitemaction.o: cocoamenuitemaction.mm cocoamenuitemaction.h
	g++ -g $(includes)  -fPIC -Wall -c cocoamenuitemaction.mm

cocoapasteboardaction.o: cocoapasteboardaction.mm cocoapasteboardaction.h
	g++ -g $(includes)  -fPIC -Wall -c cocoapasteboardaction.mm

cocoabuttonimpl.o: cocoabuttonimpl.mm cocoabuttonimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoabuttonimpl.mm

cocoaradiobuttonimpl.o: cocoaradiobuttonimpl.mm cocoaradiobuttonimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoaradiobuttonimpl.mm

cocoacheckboximpl.o: cocoacheckboximpl.mm cocoacheckboximpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoacheckboximpl.mm

cocoaopenpickerimpl.o: cocoaopenpickerimpl.mm cocoaopenpickerimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoaopenpickerimpl.mm

cocoasaveaspickerimpl.o: cocoasaveaspickerimpl.mm cocoasaveaspickerimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoasaveaspickerimpl.mm

cocoatextimpl.o: cocoatextimpl.mm cocoatextimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoatextimpl.mm

cocoastatictextimpl.o: cocoastatictextimpl.mm cocoastatictextimpl.h
	g++ -g $(includes)  -fPIC -Wall -c cocoastatictextimpl.mm

cocoabuttonaction.o: cocoabuttonaction.mm cocoabuttonaction.h
	g++ -g $(includes)  -fPIC -Wall -c cocoabuttonaction.mm

cocoaresizeaction.o: cocoaresizeaction.mm cocoaresizeaction.h
	g++ -g $(includes)  -fPIC -Wall -c cocoaresizeaction.mm

buttonpresssubject.o: ../buttonpresssubject.cpp ../buttonpresssubject.h
	g++ -g $(includes)  -fPIC -Wall -c ../buttonpresssubject.cpp

menuitemclicksubject.o: ../menuitemclicksubject.cpp ../menuitemclicksubject.h
	g++ -g $(includes)  -fPIC -Wall -c ../menuitemclicksubject.cpp

windowresizesubject.o: ../windowresizesubject.cpp ../windowresizesubject.h
	g++ -g $(includes)  -fPIC -Wall -c ../windowresizesubject.cpp

windowraisesubject.o: ../windowraisesubject.cpp ../windowraisesubject.h
	g++ -g $(includes)  -fPIC -Wall -c ../windowraisesubject.cpp

clean:
	rm -f *~ *.so *.o 
