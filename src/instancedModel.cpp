#include "instancedModel.h"

#include <vector>

#include "glm/gtc/matrix_transform.hpp"

InstancedModel::InstancedModel(
	Shader& shader, Model& model, const std::shared_ptr<Terrain>& terrain,
	int amount, const Range<int>& widthRange, const Range<int>& heightRange
) 
	: shader(shader), model(model),	terrain(terrain), amount(amount)
{
	generatePositions(widthRange, heightRange);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), modelMatrices.data(), GL_STATIC_DRAW);

	const std::size_t vec4Size{ sizeof(glm::vec4) };
	// This is for manually setting the VAOs of the models meshes
	for (unsigned int i = 0; i < model.meshes.size(); i++) {
		GLuint VAO{ model.meshes[i].getVAO() };
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		// Keep same for all vertices of the instance
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

InstancedModel::InstancedModel(Shader& shader, Model& model, const std::shared_ptr<Terrain>& terrain, int amount)
	: InstancedModel(
		shader, model, terrain, amount,
		Range<int>(-terrain->width / 2, terrain->width / 2),
		Range<int>(-terrain->height / 2, terrain->height / 2))
{}

void InstancedModel::draw() {
	shader.use();
	for (unsigned int i = 0; i < model.meshes.size(); i++) {
		// This is a simplified version for now
		shader.setInt("texture_diffuse1", 0);
		model.meshes[i].textures[0]->bind(GL_TEXTURE0);

		glBindVertexArray(model.meshes[i].getVAO());
		glDrawElementsInstanced(GL_TRIANGLES, model.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}
}

InstancedModel::~InstancedModel() {
	glDeleteBuffers(1, &buffer);
}

// This whole method is very for-purpose and therefore needs to be changed
void InstancedModel::generatePositions(const Range<int>& widthRange, const Range<int>& heightRange) {
	modelMatrices = std::vector<glm::mat4>(amount, glm::mat4(1.0f));

	for (int i = 0; i < amount; ++i) {
		// Get a position in the ranges provided
		glm::vec3 position;
		float y{ 0.0f };
		// For now a very naive check to ensure no flower are in the water
		while (y <= 0.0f) {
			position = glm::vec3(
				TerrainUtils::randGen(heightRange.min, heightRange.max),
				0.0f,
				TerrainUtils::randGen(widthRange.min, widthRange.max));

			y = TerrainUtils::terrainHeight(
				terrain->vertices, terrain->width, terrain->height, position) + model.height;
		}
		position.y += y;
		modelMatrices[i] = glm::translate(modelMatrices[i], position);
		modelMatrices[i] = glm::scale(modelMatrices[i], glm::vec3(0.2f, 0.2f, 0.2f));
	}
}
