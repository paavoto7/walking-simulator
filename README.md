# Walking simulator
A simple OpenGL-based walking simulator with terrain rendering, model loading, and basic movement mechanics.

This project was originally transferred from a repository I used while learning OpenGL with  [learnopengl.com](https://learnopengl.com/).
Since then, I've added features like heightmap loading, height-based colouring, jumping, model loading, a skybox, and more.

The main purpose of this project is simply to help me learn OpenGL and more C++. The features are still quite rudimentary,
but I hope to eventually turn it into a more feature-rich walking simulator. *(Not like those Unreal Engine demos on YouTube made by shading experts!)*

## Getting started
Before building, see [`assets/README.md`](assets/README.md) for details about the included 3D model and asset usage.
I don't currently provide the dependencies in the project so downloading and building them is left to the reader.

### Requirements
- C++17 (Should work with 14 as well)
- Visual Studio (VS 2022 was used)

### Dependencies
- [GLFW](https://github.com/glfw/glfw): Library for creating windows and handling OpenGL contexts and input.
- [Glad](https://github.com/Dav1dde/glad): Loader for OpenGL function pointers and extensions.
	- Generated with [Glad loader generator (version 1)](https://glad.dav1d.de/#profile=core&language=c&specification=gl&loader=on&api=gl%3D4.6)
- [GLM](https://github.com/g-truc/glm): Graphics-focused math library especially for vectors, matrices, and transformations.
- [stb](https://github.com/nothings/stb) (stb_image.h): A simple image loader.
- [Assimp](https://github.com/assimp/assimp): 3D model importer (e.g. .obj format).

### Building
- Get the code:
	```
	git clone https://github.com/paavoto7/walking-simulator.git
	```
- Copy `glad.c` (at least for OpenGL 3.3) to the src directory
- Build Assimp and store the DLL in the project root
- Open [walking-simulator.sln](/walking-simulator.sln) in Visual Studio to build and run the program

## Tools used
- Git
- Visual Studio 2022
- CMake
- Blender

### Backlog
- Make a CMakeLists file for portability
- Implement better water (Textures, physics etc.)
- Make the movement more natural i.e. better gravity and momentum
- Add textures to ground
- Make more models e.g. trees, houses, rocks in Blender
- Have models draw colours as well
- Maybe add procedural terrain generation with e.g. Perlin noise
- Maybe add NPCs. Will require some basic AI (state machine, behavior tree etc.), basic skeletal modeling
	and possible animations.