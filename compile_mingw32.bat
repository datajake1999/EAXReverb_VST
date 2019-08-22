@ECHO OFF
g++ -fPIC -c -IVST3_SDK -D__cdecl="" src\*.cpp VST3_SDK\public.sdk\source\vst2.x\*.cpp
g++ -s -shared -static -o EAXReverb.dll *.o
del *.o
