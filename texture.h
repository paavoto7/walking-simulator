#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.h"

class Texture {
public:
	Texture(const char* filename) {

		stbi_set_flip_vertically_on_load(true);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned char* data{ stbi_load(filename, &texWidth, &texHeight, &texNrChannels, 0) };
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load texture!" << std::endl;
		}
		stbi_image_free(data);
	}

	void bind(GLenum option) const {
		glActiveTexture(option);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void unBind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	GLuint texture;
	int texWidth, texHeight, texNrChannels;
};

#endif // !TEXTURE_H
