CC = g++
CFLAGS = -fPIC -c -I$(VSTSDK) -D__cdecl=""
LFLAGS = -shared -o $(TARGET)
VSTSDK = ../VST3_SDK
SRCS = ../src/eaxreverb.cpp ../src/main.cpp ../src/ReverbEffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffectx.cpp $(VSTSDK)/public.sdk/source/vst2.x/vstplugmain.cpp
OBJS = audioeffect.o audioeffectx.o eaxreverb.o main.o ReverbEffect.o vstplugmain.o
TARGET = EAXReverb.vst/Contents/MacOS/EAXReverb

all: compile link

compile: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

link: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS)

.PHONY: clean help

clean:
	rm -f $(OBJS) $(TARGET)

help:
	@echo supported commands:
	@echo make: compile and link the library
	@echo make compile: compile source files into object code
	@echo make link: link object code into shared library
	@echo make clean: remove object code and compiled library from working directory
	@echo make help: list supported commands
