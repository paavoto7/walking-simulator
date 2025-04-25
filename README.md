# Walking simulator
This project was originally transferred from a repository I used while learning OpenGL with  [learnopengl.com](https://learnopengl.com/).
Since then, I've added features like heightmap loading, height-based coloring, jumping, model loading, a skybox, and more.

The main purpose of this project is simply to help me learn OpenGL and more C++. The features are still quite rudimentary,
but I hope to eventually turn it into a more feature-rich walking simulator. *(Not like those stunning Unreal Engine demos on YouTube made by shading experts!)*

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
1. Have models draw colours as well
1. Maybe add procedural terrain generation with e.g. Perlin noise
1. Maybe add NPCs. Will require some basic AI (state machine, behavior tree etc.), basic skeletal modeling
	and possible animations.


See [`assets/README.md`](assets/README.md) for details about the included 3D model and asset usage.