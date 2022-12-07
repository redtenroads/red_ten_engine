# Red Ten Engine
### Is
Personal 2d/3d game engine I'm creating for development of my indie projects. Currently in alpha state, use on your own risk, existing components are enough to make games, but it's not enough for AAA quality

### Features
* Modern deffered rendering allowing hundreds of light sources to be rendered without fps drops. Shadows supported
* Layers system allowing to separate actors, effects, sprites and UI by using different layers and managing them separately. Additional libraries like UI may exist on their own layer without collisions with your 3d objects/sprites and other UI implementations. Same for the effect libraries
* 2d/3d Physics are managed by Jolt game physics engine released under MIT license
* Free for commercial and none commercial usage (MIT license)

### Compilation
1. Clone the repo, install make if you don't have it, install clang++
2. Download and install following libraries: 
* Jolt physics engine - https://github.com/jrouwe/JoltPhysics
* SDL 2 - https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0
* SDL 2 ttf - https://github.com/libsdl-org/SDL_ttf/releases
* OpenAL - https://www.openal.org/downloads/
3. In the makefile provide pathes to this libraries
4. Verify library versions in bin directory or simply replace dlls there with the ones provided with the installations
5. Run "make" in root directory

### Usage
Link rtengine.lib with your program and put rtengine.dll, SDL2_ttf.dll, SDL2.dll, zlib1.dll, libfreetype-6.dll into your bin folder. You can follow building process for examples
Note, that opanAL libraries is usuall presented on the systems, but might be missing. Installer is on their official website https://www.openal.org/downloads/

### How to learn
The easiest and currently the only way is to go through the examples. They have comments to explain how to do most of the things the engine capable of and to understand it's architecture

### How engine is being developed
I have a huge backlog of features I want to implement. I select them mostly by my current needs of the projects I'm working on. There is no public backlog at the moment

### Todo
* Public list of planned features
* Paragraph in readme about contribution
* Some showcase of examples, probably video
* List of current limitations