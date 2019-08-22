CC = g++
CFLAGS = -fPIC -c -I$(VSTSDK) -D__cdecl=""
LFLAGS = -shared -o $(TARGET)
VSTSDK = VST3_SDK
SRCS = src/eaxreverb.cpp src/main.cpp src/ReverbEffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffectx.cpp $(VSTSDK)/public.sdk/source/vst2.x/vstplugmain.cpp
OBJS = audioeffect.o audioeffectx.o eaxreverb.o main.o ReverbEffect.o vstplugmain.o
TARGET = EAXReverb.so

all: eaxreverb_compile eaxreverb_link

eaxreverb_compile: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

eaxreverb_link: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)