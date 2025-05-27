#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>

#include <glad/glad.h>

#include "game_object.h"
#include "shader.h"

// Generates the terrain from a heightmap
class Terrain: public GameObject {
public:
	int width, height, nrChannels;
	std::vector<float> vertices;

	Terrain(Shader& shader);

	~Terrain();

	void draw() override;

	void loadHeightMap();

private:
	GLuint groundVBO;
	GLuint groundVAO;
	GLuint groundEBO;
	Shader& shader;

	unsigned NUM_STRIPS{};
	unsigned NUM_VERTS_PER_STRIP{};
	// Affects the height of the terrain
	const float yScale{ 64.0f / 256.0f };
	const float yShift{ 16.0f };

	std::vector<unsigned> indices;
};

#endif // !TERRAIN_H
