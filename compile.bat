cl -c /I ".\VST3 SDK" /I ".\include" "VST3 SDK\public.sdk\source\vst2.x\*.cpp" "src\*.cpp"
link *.obj bufferoverflowU.lib kernel32.lib user32.lib /dll /def:".\src\vstplug.def" /OUT:EAXReverb.dll
del *.obj
