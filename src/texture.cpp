#include "texture.h"

#include <iostream>

#include "stb_image.h"

Texture::Texture(const char* filename, bool flipOnLoad, GLenum textureType) : textureType(textureType) {
	stbi_set_flip_vertically_on_load(flipOnLoad);

	glGenTextures(1, &id);
	glBindTexture(textureType, id);

	unsigned char* data{ stbi_load(filename, &texWidth, &texHeight, &texNrChannels, 0) };
	if (data) {
		GLenum format{ GL_NONE };
		if (texNrChannels == 1) {
			format = GL_RED;
		}
		else if (texNrChannels == 3) {
			format = GL_RGB;
		}
		else if (texNrChannels == 4) {
			format = GL_RGBA;
		}
		else {
			std::cout << "Unsupported texture format with " << texNrChannels << "channels" << std::endl;
			return;
		}

		glTexImage2D(textureType, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(textureType);

		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else {
		std::cout << "Failed to load texture!" << std::endl;
	}

	stbi_image_free(data);
}

// Used when calling from Model with texture data from Assimp models
Texture::Texture(const char* filename, const std::string& dir, bool flipOnLoad)
	: Texture((dir + '/' + filename).c_str(), flipOnLoad) {
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

Texture::Texture(Texture&& other) noexcept
	: id(other.id),
	textureType(other.textureType),
	texWidth(other.texWidth),
	texHeight(other.texHeight),
	texNrChannels(other.texNrChannels),
	type(std::move(other.type)),
	path(std::move(other.path))
{
	other.id = 0; // prevent deletion in moved-from object
}

Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		id = other.id;
		textureType = other.textureType;
		texWidth = other.texWidth;
		texHeight = other.texHeight;
		texNrChannels = other.texNrChannels;
		type = std::move(other.type);
		path = std::move(other.path);

		other.id = 0;
	}
	return *this;
}

void Texture::bind(GLenum option) const {
	glActiveTexture(option);
	glBindTexture(textureType, id);
}

void Texture::unBind() const {
	glBindTexture(textureType, 0);
}
