# AWayBack.Tools

A set of tools used internally to create [A Way Back Game](https://store.steampowered.com/app/1569170/A_Way_Back/).

## Sprite atlas editor

One of the most common things used in the 2D gamedev are packing multiple sprites into on single [sprite atlas](https://en.wikipedia.org/wiki/Texture_atlas).

There are different approaches to accomplish this:

1. Have different sprites as separate files then pack them into one file during a build via a script or some other pipeline.
2. Have a prebaked texture with multiple sprites and slice them into sprite atlas. Basically having a configuration file having an array of entries pointing to a region of a big texture (a sprite region).

I chose the second approach as it felt simpler. Thus a sprite editor came to be.

### Requirements:
- Win x64 OS. Linux/Mac might be coming soon(TM)
- OpenGL 3.3 or higher support

### Features:

- Slicing a sequence of sprites on a grid
- Slicing individual cells on a grid
- Slicing a freehand region
- Editing an origin aka pivot of a sprite
- Renaming a sprite
- Saving to a JSON file
- Undo/Redo

### Downloading source and building
- Clone the repo

`git clone https://github.com/hazevich/AWayBack.Tools`

- Generate project files with premake5 for your beloved IDE or code editor. Here we'll use Visual Studio 2019

`premak5 vs2019`

- Run `AWayBack.Tools.sln`
- Build