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
	
	Texture(const char* filename, GLenum textureType = GL_TEXTURE_2D): textureType(textureType) {
		stbi_set_flip_vertically_on_load(true);

		glGenTextures(1, &id);
		glBindTexture(textureType, id);

		unsigned char* data{ stbi_load(filename, &texWidth, &texHeight, &texNrChannels, 0) };
		if (data) {
			GLenum format{ GL_NONE };
			if (texNrChannels == 1) {
				format = GL_RED;
			} else if (texNrChannels == 3) {
				format = GL_RGB;
			} else if (texNrChannels == 4) {
				format = GL_RGBA;
			} else {
				std::cout << "Unsupported texture format with " << texNrChannels << "channels" << std::endl;
				return;
			}

			glTexImage2D(textureType, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(textureType);

			glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		} else {
			std::cout << "Failed to load texture!" << std::endl;
		}

		stbi_image_free(data);
	}

	// Used when calling from Model with texture data from Assimp models
	Texture(const char* filename, const std::string& dir)
		: Texture((dir + '/' + filename).c_str()) {}

	~Texture() {
		glDeleteTextures(1, &id);
	}

	void bind(GLenum option) const {
		glActiveTexture(option);
		glBindTexture(textureType, id);
	}

	void unBind() const {
		glBindTexture(textureType, 0);
	}

private:
	GLuint id;
	GLenum textureType;
	int texWidth, texHeight, texNrChannels;
};

#endif // !TEXTURE_H
