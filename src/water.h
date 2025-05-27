#ifndef WATER_H
#define WATER_H

#include <vector>

#include <glad/glad.h>

#include "game_object.h"
#include "shader.h"

// Class to represent water
class Water: public GameObject {
public:
	Water(const std::vector<float> vertices, Shader& shader);

	~Water();

	void draw() override;
private:
	GLuint VAO, VBO;
	std::vector<float> vertices;
	int vertexCount;
	Shader& shader;
};

#endif // !WATER_H
