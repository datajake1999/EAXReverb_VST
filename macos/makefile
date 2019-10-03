CC = g++
CFLAGS = -fPIC -c -I$(VSTSDK) -D__cdecl=""
LFLAGS = -shared -o $(TARGET)
VSTSDK = ../VST3_SDK
SRCS = ../src/eaxreverb.cpp ../src/main.cpp ../src/ReverbEffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffectx.cpp $(VSTSDK)/public.sdk/source/vst2.x/vstplugmain.cpp
OBJS = audioeffect.o audioeffectx.o eaxreverb.o main.o ReverbEffect.o vstplugmain.o
TARGET = dist/EAXReverb.vst/Contents/MacOS/EAXReverb

all: compile link

compile: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

link: $(OBJS)
	mkdir dist/EAXReverb.vst/Contents/MacOS
	$(CC) $(LFLAGS) $(OBJS)

dmg: $(TARGET)
	hdiutil create -volname EAXReverb -srcfolder dist -ov -format UDZO EAXReverb.dmg

.PHONY: clean help

clean:
	rm -f $(OBJS) $(TARGET)

help:
	@echo supported commands:
	@echo make: compile and link the library
	@echo make compile: compile source files into object code
	@echo make link: link object code into shared library
	@echo make dmg: create disk image for distribution
	@echo make clean: remove object code and compiled library from working directory
	@echo make help: list supported commands
