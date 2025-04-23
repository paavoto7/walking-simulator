#include "mesh.h"

#include <string>

Mesh::Mesh(
	const std::vector<Vertex>& vertices,
	const std::vector<unsigned>& indices,
	const std::vector<std::shared_ptr<Texture>>& textures
) 
	: vertices(vertices), indices(indices), textures(textures)
{
	setupMesh();
}

Mesh::Mesh(Mesh&& other) noexcept
	: vertices(std::move(other.vertices)),
	indices(std::move(other.indices)),
	textures(std::move(other.textures)),
	VAO(other.VAO),
	VBO(other.VBO),
	EBO(other.EBO)
{
	// Invalidate the other Mesh's OpenGL handles so its destructor won't delete them
	other.VAO = 0;
	other.VBO = 0;
	other.EBO = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
	if (this != &other) {
		// Delete the current objects buffers if they exist
		if (VAO) glDeleteVertexArrays(1, &VAO);
		if (VBO) glDeleteBuffers(1, &VBO);
		if (EBO) glDeleteBuffers(1, &EBO);

		// Transfer the data and buffers
		vertices = std::move(other.vertices);
		indices = std::move(other.indices);
		textures = std::move(other.textures);
		VAO = other.VAO;
		VBO = other.VBO;
		EBO = other.EBO;

		// Make the other buffers invalid so others destructor doesn't delete ones in use
		other.VAO = 0;
		other.VBO = 0;
		other.EBO = 0;
	}
	return *this;
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::draw(const Shader& shader) {
	unsigned int diffuseNr{ 1 };
	unsigned int specularNr{ 1 };
	// This simply goes over the textures and sets the proper ones
	for (unsigned int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name{ textures[i]->type };
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}
		// Gives a formatted and relatively easy way to use in shaders
		shader.setInt(name + number, i);
		textures[i]->bind(GL_TEXTURE0 + i);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

GLuint Mesh::getVAO() const {
	return VAO;
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}