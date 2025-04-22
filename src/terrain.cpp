#include "terrain.h"

#include <iostream>

#include "stb_image.h"

Terrain::Terrain() {
	loadHeightMap();
	if (vertices.empty()) return;

	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glGenBuffers(1, &groundEBO);

	glBindVertexArray(groundVAO);

	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Terrain::~Terrain() {
	glDeleteVertexArrays(1, &groundVAO);
	glDeleteBuffers(1, &groundVBO);
	glDeleteBuffers(1, &groundEBO);
}

void Terrain::draw() {
	glBindVertexArray(groundVAO);
	// Loop over the strips to draw the terrain
	for (unsigned strip = 0; strip < NUM_STRIPS; ++strip) {
		glDrawElements(
			GL_TRIANGLE_STRIP,
			NUM_VERTS_PER_STRIP,
			GL_UNSIGNED_INT,
			reinterpret_cast<void*>((sizeof(unsigned) * NUM_VERTS_PER_STRIP * strip))
		);
	}
}

void Terrain::loadHeightMap() {
	stbi_set_flip_vertically_on_load(true);

	unsigned char* heightMapData{ stbi_load("assets/heightMap.png", &width, &height, &nrChannels, 0) };

	if (heightMapData) {
		std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
	}
	else {
		std::cout << "Failed to load heightmap" << std::endl;
		return;
	}

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			unsigned char* texel{ heightMapData + (width * i + j) * nrChannels };

			unsigned char y = *texel;

			// Centered at (0,y,0)
			vertices.push_back(-height / 2.0f + i); // x
			vertices.push_back(static_cast<int>(y) * yScale - yShift); // y
			vertices.push_back(-width / 2.0f + j); // z
		}
	}

	std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;

	stbi_image_free(heightMapData);

	// Strip rendering goes over two rows at a time, therefore:
	NUM_STRIPS = height - 1;
	NUM_VERTS_PER_STRIP = width * 2;

	for (unsigned i = 0; i < NUM_STRIPS; ++i) {
		for (unsigned j = 0; j < width; ++j) {
			for (unsigned k = 0; k < 2; ++k) {
				// One from current row, one from next
				indices.push_back(j + width * (i + k));
			}
		}
	}
}
