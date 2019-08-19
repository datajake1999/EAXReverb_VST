[Setup]
AppName=EAXReverb VST Plugin
AppVerName=EAXReverb VST Plugin
CreateAppDir=no
OutputBaseFilename=EAXReverb_VST
ArchitecturesInstallIn64BitMode=X64

[Files]
Source: "EAXReverb.dll"; DestDir: "{pf32}\Steinberg\VstPlugins"
Source: "EAXReverb_x64.dll"; DestDir: "{pf}\Steinberg\VstPlugins"; Check: Is64BitInstallMode
