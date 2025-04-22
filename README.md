# Walking simulator

This project is copied from my repository I used to learn OpenGL with [learnopengl.com](https://learnopengl.com/).
Now other details such as heightmap reading, coloring it based on height, jumping and such have been added.
The features are still very rudimentary, but I hope I can make a nice little walking simulator out of this.
(Not those demos you see in YouTube that made in Unreal Engine by shading experts)

## Dependencies
- [GLFW](https://github.com/glfw/glfw)
- [Glad](https://github.com/Dav1dde/glad)
- [GLM](https://github.com/g-truc/glm)
- [stb](https://github.com/nothings/stb) (stb_image.h)
- [Assimp](https://github.com/assimp/assimp)

## Tools used
- Git
- Visual Studio 2022
- Cmake
- Blender

### Backlog
1. Implement better water (Textures, physics etc.)
1. Implement instanced rendering for models and randomly assign positions
1. Make the movement more natural i.e. better gravity and momentum
1. Add textures to ground
1. Make more models e.g. trees, houses, rocks in Blender
1. Make models draw colours as well
1. Maybe add procedural terrain generation with e.g. Perlin noise
1. Maybe add NPCs. Will require some basic AI (state machine, behavior tree etc.), basic skeletal modeling
	and possible animations.