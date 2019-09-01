# Introduction
This is a VST effect that emulates EAX (Environmental Audio Extensions) reverb. EAX was a standard used in late 90s to early 2000s Creative Labs sound cards to add audio effects to games. Some modern sound cards such as Realtek HD Audio can emulate EAX. This VST is based on the [openal-soft](https://github.com/kcat/openal-soft) EAX Reverb Implementation which was [ported to c++](https://github.com/Relfos/EAXReverb).

# What you can use this for
With this VST, you are not limited to games. No matter if you are working on a production and want to add some reverb, or working on a game, reverb is essential. Do your vocals or drums sound too dry? This plugin can put them in a virtual room. Are you developing a game that uses EAX and want to make your own presets? You can use this VST to help develop EAX presets for your games.

# How to use
to use this plugin, any VST2 compatible host will work. Here's a list of some hosts that you can use

* Reaper
* Ableton live
* pro tools
* fl studio

When you open the plugin, you will notice there is no UI, so you will have to use your DAW's method of setting effect parameters.
It is very easy to utilize this plugin. Lets use Reaper as an example.
In your parameter window, you will see a list of parameters and their values. You can scroll with your mouse or use your keyboard to choose your parameter/value, and if you know which value you want right away, you can type it in the value edit field.

# Building from source

## on Windows
To build this effect from source, you will need:

* Microsoft Visual Studio 2005 Express
* Windows Server 2003 Platform SDK
* VST SDK
* InnoSetup
* 7-Zip

To build:

* Copy the VST SDK folder to the location of the source code.
* Open a command prompt and run compile_msvc.bat.
* If everything goes well, you should have DLL's that you can load into your favorite VST host, and an installer that you can use to install the plugin to the standard location for VST plugins.

## on Linux
To build this effect from source, you will need:

* g++
* make
* VST SDK
* gzip (optional)

To build:

* Copy the VST SDK folder to the location of the source code.
* Open a terminal window and cd to the directory that contains the source.
* Type "make" to build the library.
* type "make install" to install the library onto the system.

# License
This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

# Credits
* @kcat, for writing openal-soft.
* @Relfos, for porting the EAX Reverb code to C++.
* @gijzelaerr, for writing a simple VST example.
* @kn0ll, for providing the official VST 2.4 examples.
* Me, for putting all of this together into a VST Plugin.
* @DannyBoy96, for helping with the read-me.

# Useful links
* [Environmental Audio Extensions on Wikipedia](https://en.wikipedia.org/wiki/Environmental_Audio_Extensions)
* [Setting Reverb Data of Rooms (EAX) on The DarkMod Wiki](http://wiki.thedarkmod.com/index.php?title=Setting_Reverb_Data_of_Rooms_(EAX))

Have fun!
