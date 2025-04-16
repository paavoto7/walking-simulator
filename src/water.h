#ifndef WATER_H
#define WATER_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_object.h"

// Class to represent water
class Water: public GameObject {
public:
	std::vector<float> vertices;

	Water(std::vector<float> vertices): vertices(vertices) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Not in use for now, but data has them
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	~Water() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void draw() override {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}
private:
	GLuint VAO, VBO;
};

#endif // !WATER_H
