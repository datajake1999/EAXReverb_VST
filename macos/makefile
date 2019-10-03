CC = g++
CFLAGS = -fPIC -c -I$(VSTSDK) -D__cdecl=""
LFLAGS = -shared -o $(TARGET)
VSTSDK = ../VST3_SDK
SRCS = ../src/eaxreverb.cpp ../src/main.cpp ../src/ReverbEffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffectx.cpp $(VSTSDK)/public.sdk/source/vst2.x/vstplugmain.cpp
OBJS = audioeffect.o audioeffectx.o eaxreverb.o main.o ReverbEffect.o vstplugmain.o
TARGET = ROOT/Library/Audio/Plug-ins/VST/EAXReverb.vst/Contents/MacOS/EAXReverb
PKG = pkg/EAXReverb.pkg

all: compile link pkg dmg

compile: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

link: $(OBJS)
	mkdir ROOT/Library/Audio/Plug-ins/VST/EAXReverb.vst/Contents/MacOS
	$(CC) $(LFLAGS) $(OBJS)

pkg: $(TARGET)
	mkdir pkg
	pkgbuild --root ROOT --identifier com.Datajake.EAXReverb --version 1.0 $(PKG)

dmg: $(PKG)
	hdiutil create -volname EAXReverb -srcfolder pkg -ov -format UDZO EAXReverb.dmg

.PHONY: clean help

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
	@echo make help: list supported commands
