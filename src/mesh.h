#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

// Collect all data of a single vertex
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

// Used with models loaded with Assimp
class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<std::shared_ptr<Texture>> textures);

	// Disallow copying to avoid destructor problems
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept;

	Mesh& operator=(Mesh&& other) noexcept;
	
	~Mesh();

	void draw(const Shader& shader);

private:
	GLuint VAO, VBO, EBO;

	void setupMesh();
};


#endif // !MESH_H
