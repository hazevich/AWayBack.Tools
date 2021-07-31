# AWayBack.Tools

A set of tools used internally to create [A Way Back Game](https://store.steampowered.com/app/1569170/A_Way_Back/).

## Sprite atlas editor

One of the most common things used in the 2D gamedev are packing multiple sprites into on single [sprite atlas](https://en.wikipedia.org/wiki/Texture_atlas).

There are different approaches to accomplish the result having a manually baked texture and slice them into sprite atlas with a tool.

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

- Generate project files with [premake5](https://premake.github.io/) for your beloved IDE or code editor. Here we'll use Visual Studio 2019

`premake5 vs2019`

You have to download premake if you don't want to. There is `premake5.exe` included the repo.

- Run `AWayBack.Tools.sln`
- Build
