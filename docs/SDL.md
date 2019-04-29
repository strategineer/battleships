SDL
===
## Installation
Use the below tutorial to add SDL2 to a new project in Visual Studio 2012/2013
http://jskiles1.wordpress.com/2013/09/04/setting-up-sdl2-with-visual-studio-2012/
*Note*: On the last step (13), I had to use the .dll found in the lib/x86 folder to get the .exe to run.

In order to install the extensions as well, use this tutorial http://twinklebear.github.io/sdl2%20tutorials/2013/08/18/lesson-3-sdl-extension-libraries/ .
*Note*: Basically, you download each extension's Visual Studio/VC zip file and unpack each into your /SDL/ folder and finally add a Linker reference to each .lib (I might have forgotten something). If you want to get the program to run you also have to put the .dll in the same folder (Debug/Release) as your .exe executable.

## Helpful Links
API: http://wiki.libsdl.org/APIByCategory
SDL_Event: http://wiki.libsdl.org/SDL_Event
SDL 2.0 Tutorial: http://lazyfoo.net/tutorials/SDL/