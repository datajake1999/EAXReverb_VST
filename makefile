CC = g++
CFLAGS = -fPIC -c -I$(VSTSDK) -D__cdecl=""
LFLAGS = -shared -Wl,-soname,$(TARGET) -o $(TARGET)
VSTSDK = VST3_SDK
SRCS = src/biquad.cpp src/bitcrush.cpp src/eaxreverb.cpp src/LinearResampler.cpp src/main.cpp src/resampler.cpp src/ReverbEffect.cpp src/silence.cpp src/ZOHResampler.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffectx.cpp $(VSTSDK)/public.sdk/source/vst2.x/vstplugmain.cpp
OBJS = audioeffect.o audioeffectx.o biquad.o bitcrush.o eaxreverb.o LinearResampler.o main.o resampler.o ReverbEffect.o silence.o vstplugmain.o ZOHResampler.o 
TARGET = EAXReverb.so
INSTDIR = /usr/lib/vst

all: compile link

compile: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

link: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS)

install: $(TARGET)
	mkdir $(INSTDIR)
	cp $(TARGET) $(INSTDIR)

uninstall: $(INSTDIR)/$(TARGET)
	rm $(INSTDIR)/$(TARGET)

compress: $(TARGET)
	gzip $(TARGET)

decompress: $(TARGET).gz
	gzip -d $(TARGET).gz

.PHONY: clean help license

clean:
	rm -f $(OBJS) $(TARGET)

help:
	@echo supported commands:
	@echo make: compile and link the library
	@echo make compile: compile source files into object code
	@echo make link: link object code into shared library
	@echo make install: install the library onto the system
	@echo make uninstall: remove the library from the system
	@echo make compress: compress the library into a gzip archive
	@echo make decompress: decompress the library from a gzip archive
	@echo make clean: remove object code and compiled library from working directory
	@echo make license: display license
	@echo make help: list supported commands

license:
	@echo "This library is free software; you can redistribute it and/or"
	@echo "modify it under the terms of the GNU Lesser General Public"
	@echo "License as published by the Free Software Foundation; either"
	@echo "version 2.1 of the License, or (at your option) any later version."
