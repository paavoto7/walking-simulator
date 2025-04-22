#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

#include "stb_image.h"
#include "shader.h"

class Texture {
public:
	// These are for Assimp loaded textures
	std::string type;
	std::string path;
	
	Texture(const char* filename, bool flipOnLoad = true, GLenum textureType = GL_TEXTURE_2D);

	// Used when calling from Model with texture data from Assimp models
	Texture(const char* filename, const std::string& dir, bool flipOnLoad = true)
		: Texture((dir + '/' + filename).c_str(), flipOnLoad) {}


	// Move constructor
	Texture(Texture&& other) noexcept;

	// Move assignment
	Texture& operator=(Texture&& other) noexcept;

	// Delete copy constructor and assignment to prevent accidental copies
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	~Texture() {
		glDeleteTextures(1, &id);
	}

	void bind(GLenum option) const;

	void unBind() const;

private:
	GLuint id;
	GLenum textureType;
	int texWidth, texHeight, texNrChannels;
};

#endif // !TEXTURE_H
