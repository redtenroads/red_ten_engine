# Red Ten Engine
### Is
2d/3d game engine used by Red Ten Roads studio to create games. Currently in alpha state, use on your own risk, but existing components are already enough to make games.

### Compilation
1. Clone the repo, install make if you don't have it, install clang++.
2. Download and install following libraries: 
* Jolt physics engine - https://github.com/jrouwe/JoltPhysics
* SDL 2 - https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0
* SDL 2 ttf - https://github.com/libsdl-org/SDL_ttf/releases
* OpenAL - https://www.openal.org/downloads/
3. In the makefile provide pathes to this libraries
4. Verify library versions in bin directory or simply replace dlls there with the ones provided with the installations
5. Run "make" in root directory

### Usage
Link rtengine.lib with your program and put rtengine.dll, SDL2_ttf.dll, SDL2.dll, zlib1.dll, libfreetype-6.dll into your execution folder.
Note, that opanAL libraries is usuall presented on the systems, but might be missing. Installer is on their official website https://www.openal.org/downloads/

### How to learn
The easiest and the only way is to go through the examples. They have comments to explain how to do most of the things with the engine and to understand it's architecture.

### How engine is being developed
First of all I modify engine to fulfill my needs in my own game development so features depends on what I personally need.

### Todo
* Public list of planned features
* Paragraph in readme about contribution
* Some showcase of examples, probably video