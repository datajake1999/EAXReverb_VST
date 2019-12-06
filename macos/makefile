CC = g++
CFLAGS = -fPIC -c -I$(VSTSDK) -D__cdecl=""
LFLAGS = -shared -o $(TARGET)
VSTSDK = ../VST3_SDK
SRCS = ../src/biquad.cpp ../src/bitcrush.cpp ../src/eaxreverb.cpp ../src/LinearResampler.cpp ../src/main.cpp ../src/resampler.cpp ../src/ReverbEffect.cpp ../src/silence.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffectx.cpp $(VSTSDK)/public.sdk/source/vst2.x/vstplugmain.cpp
OBJS = audioeffect.o audioeffectx.o biquad.o bitcrush.o eaxreverb.o LinearResampler.o main.o resampler.o ReverbEffect.o silence.o vstplugmain.o
TARGET = EAXReverb.vst/Contents/MacOS/EAXReverb
PKG = pkg/EAXReverb.pkg

all: compile link pkg dmg

compile: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

link: $(OBJS)
	mkdir EAXReverb.vst/Contents/MacOS
	$(CC) $(LFLAGS) $(OBJS)

pkg: $(TARGET)
	mkdir pkg
	productbuild --component EAXReverb.vst /Library/Audio/Plug-ins/VST $(PKG)

dmg: $(PKG)
	cp ../LICENSE.TXT pkg
	cp ../readme.md pkg
	hdiutil create -volname EAXReverb -srcfolder pkg -ov -format UDZO EAXReverb.dmg

.PHONY: clean help license

clean:
	rm -f $(OBJS) $(TARGET)

help:
	@echo supported commands:
	@echo make: build everything
	@echo make compile: compile source files into object code
	@echo make link: link object code into shared library
	@echo make pkg: create installer package
	@echo make dmg: create disk image
	@echo make clean: remove object code and compiled library from working directory
	@echo make license: display license
	@echo make help: list supported commands

license:
	@echo "This library is free software; you can redistribute it and/or"
	@echo "modify it under the terms of the GNU Lesser General Public"
	@echo "License as published by the Free Software Foundation; either"
	@echo "version 2.1 of the License, or (at your option) any later version."
