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
	
	Texture(const char* filename) {
		stbi_set_flip_vertically_on_load(true);

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

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

			glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void unBind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	GLuint id;
	int texWidth, texHeight, texNrChannels;
};

#endif // !TEXTURE_H
