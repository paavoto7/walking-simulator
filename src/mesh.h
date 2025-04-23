#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>

#include <glad/glad.h>

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

	Mesh(
		const std::vector<Vertex>& vertices,
		const std::vector<unsigned>& indices,
		const std::vector<std::shared_ptr<Texture>>& textures
	);

	// Disallow copying to avoid destructor problems
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept;

	Mesh& operator=(Mesh&& other) noexcept;
	
	~Mesh();

	void draw(const Shader& shader);

	GLuint getVAO() const;

private:
	GLuint VAO, VBO, EBO;

	void setupMesh();
};


#endif // !MESH_H
