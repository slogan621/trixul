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

includes=-I../../dist/include/nspr -I../ -I../jsengine -I../jsengine/include
libs=../../dist/linux/debug/
cflags=`gtk-config --cflags`

all: libgtkwidget.so

libgtkwidget.so: gtkfactory.o gtkbuttonimpl.o gtkwindowimpl.o gtkappimpl.o \
gtktextimpl.o gtkstatictextimpl.o gtkwidgetimpl.o gtkmenuimpl.o \
gtkmenubarimpl.o windowresizesubject.o buttonpresssubject.o gtkmenuitemimpl.o \
menuitemclicksubject.o gtkopenpickerimpl.o gtksaveaspickerimpl.o \
windowraisesubject.o gtkscrolledwindowimpl.o gtkscrolledviewimpl.o \
gtkgridlistimpl.o gtkradiobuttonimpl.o gtkcheckboximpl.o radiogroupmanager.o \
clipboardhelper.o
	g++ `gtk-config --libs` -g -shared -fPIC -o libgtkwidget.so gtkfactory.o \
gtkbuttonimpl.o gtkwindowimpl.o gtkappimpl.o gtktextimpl.o gtkstatictextimpl.o \
gtkwidgetimpl.o windowresizesubject.o buttonpresssubject.o gtkmenuimpl.o \
gtkmenubarimpl.o gtkmenuitemimpl.o menuitemclicksubject.o gtkopenpickerimpl.o \
gtksaveaspickerimpl.o windowraisesubject.o gtkscrolledwindowimpl.o \
gtkgridlistimpl.o gtkscrolledviewimpl.o gtkradiobuttonimpl.o \
gtkcheckboximpl.o radiogroupmanager.o clipboardhelper.o
	cp libgtkwidget.so $(libs) 

gtkfactory.o: gtkfactory.cpp gtkfactory.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkfactory.cpp

gtkwindowimpl.o: gtkwindowimpl.cpp gtkwindowimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkwindowimpl.cpp

gtkappimpl.o: gtkappimpl.cpp gtkappimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkappimpl.cpp

gtkwidgetimpl.o: gtkwidgetimpl.cpp gtkwidgetimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkwidgetimpl.cpp

gtkopenpickerimpl.o: gtkopenpickerimpl.cpp gtkopenpickerimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkopenpickerimpl.cpp

gtkgridlistimpl.o: gtkgridlistimpl.cpp gtkgridlistimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkgridlistimpl.cpp

gtksaveaspickerimpl.o: gtksaveaspickerimpl.cpp gtksaveaspickerimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtksaveaspickerimpl.cpp

gtkmenuimpl.o: gtkmenuimpl.cpp gtkmenuimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkmenuimpl.cpp

gtkmenubarimpl.o: gtkmenubarimpl.cpp gtkmenubarimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkmenubarimpl.cpp

radiogroupmanager.o: ../radiogroupmanager.cpp ../radiogroupmanager.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c ../radiogroupmanager.cpp

gtkmenuitemimpl.o: gtkmenuitemimpl.cpp gtkmenuitemimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkmenuitemimpl.cpp

gtkscrolledwindowimpl.o: gtkscrolledwindowimpl.cpp gtkscrolledwindowimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkscrolledwindowimpl.cpp

gtkscrolledviewimpl.o: gtkscrolledviewimpl.cpp gtkscrolledviewimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkscrolledviewimpl.cpp

gtkbuttonimpl.o: gtkbuttonimpl.cpp gtkbuttonimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkbuttonimpl.cpp

gtktextimpl.o: gtktextimpl.cpp gtktextimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtktextimpl.cpp

gtkstatictextimpl.o: gtkstatictextimpl.cpp gtkstatictextimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkstatictextimpl.cpp

gtkradiobuttonimpl.o: gtkradiobuttonimpl.cpp gtkradiobuttonimpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkradiobuttonimpl.cpp

gtkcheckboximpl.o: gtkcheckboximpl.cpp gtkcheckboximpl.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c gtkcheckboximpl.cpp

buttonpresssubject.o: ../buttonpresssubject.cpp ../buttonpresssubject.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c ../buttonpresssubject.cpp

windowresizesubject.o: ../windowresizesubject.cpp ../windowresizesubject.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c ../windowresizesubject.cpp

windowraisesubject.o: ../windowraisesubject.cpp ../windowraisesubject.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c ../windowraisesubject.cpp

menuitemclicksubject.o: ../menuitemclicksubject.cpp ../menuitemclicksubject.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c ../menuitemclicksubject.cpp

clipboardhelper.o: ../clipboardhelper.cpp ../clipboardhelper.h
	g++ -g $(includes) $(cflags) -fPIC -Wall -c ../clipboardhelper.cpp

clean:
	rm -f *~ *.so *.o 
