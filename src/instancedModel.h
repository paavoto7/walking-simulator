#ifndef INSTANCEDMODEL_H
#define INSTANCEDMODEL_H

#include <memory>

#include <glm/glm.hpp>

#include "terrainUtils.h"
#include "game_object.h"
#include "shader.h"
#include "terrain.h"
#include "model.h"

using TerrainUtils::Range;

// Is intended to own a model to draw instances of
class InstancedModel: public GameObject {
public:
	InstancedModel(
		Shader& shader,
		Model& model,
		const std::shared_ptr<Terrain>& terrain,
		int amount,
		const Range<int>& widthRange,
		const Range<int>& heightRange
	);
	
	InstancedModel(Shader& shader, Model& model, const std::shared_ptr<Terrain>& terrain, int amount);

	~InstancedModel();

	// No moving or copying as of now to prevent premature buffer deletion
	InstancedModel(const InstancedModel&) = delete;
	InstancedModel& operator=(const InstancedModel&) = delete;
	// Will add move functionality when required
	InstancedModel(const InstancedModel&&) = delete;
	InstancedModel&& operator=(const InstancedModel&&) = delete;

	void draw();

private:
	Shader& shader;
	GLuint buffer;
	Model& model;
	int amount;
	std::vector<glm::mat4> modelMatrices;
	std::shared_ptr<Terrain> terrain;

	// Generate random positions for the model matrices
	void generatePositions(const Range<int>& widthRange, const Range<int>& heightRange);

};

#endif // !INSTANCEDMODEL_H
