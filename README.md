# Walking simulator
This project was initially transferred over from my repository I used to learn OpenGL with [learnopengl.com](https://learnopengl.com/).
Now other features such as heightmap reading, coloring it based on height, jumping, model loading, skybox and more have been added.

The main purpose of this project is simply to teach me OpenGL. The features are still very rudimentary,
but I hope I can make a nice little walking simulator out of this.
(Not those demos you see in YouTube that made in Unreal Engine by shading experts)

## Dependencies
- [GLFW](https://github.com/glfw/glfw): Library for creating windows and handling OpenGL contexts and input.
- [Glad](https://github.com/Dav1dde/glad): Loader for OpenGL function pointers and extensions.
- [GLM](https://github.com/g-truc/glm): Graphics-focused math library especially for vectors, matrices, and transformations.
- [stb](https://github.com/nothings/stb) (stb_image.h): A simple image loader.
- [Assimp](https://github.com/assimp/assimp): 3D model importer (e.g. .obj format).

## Tools used
- Git
- Visual Studio 2022
- CMake
- Blender

### Backlog
1. Make a CMakeLists file for portability
1. Improve encapsulation of member variables i.e. introduce getters and setters
1. Implement better water (Textures, physics etc.)
1. Make the movement more natural i.e. better gravity and momentum
1. Add textures to ground
1. Make more models e.g. trees, houses, rocks in Blender
1. Make models draw colours as well
1. Maybe add procedural terrain generation with e.g. Perlin noise
1. Maybe add NPCs. Will require some basic AI (state machine, behavior tree etc.), basic skeletal modeling
	and possible animations.