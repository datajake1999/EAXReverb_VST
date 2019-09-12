@ECHO OFF
g++ -fPIC -c -IVST3_SDK -D__cdecl="" src\*.cpp VST3_SDK\public.sdk\source\vst2.x\*.cpp
windres res\EAXReverb.rc resource.o
g++ -s -shared -static -o EAXReverb_x64.dll *.o
del *.o
