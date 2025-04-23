#ifndef TERRAINUTILS_H
#define TERRAINUTILS_H

#include <vector>
#include <random>

#include <glm/glm.hpp>

namespace TerrainUtils {

	// A simple min, max range wrapper
	template <typename T,typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	struct Range {
		T min;
		T max;

		Range(T min, T max): min(min), max(max) {}
	};

	// Determine the the height of the terrain at position x, z
	inline float terrainHeight(const std::vector<float>& vertices, int width, int height, const glm::vec3& position) {
		int vertX{ static_cast<int>(position.x + height / 2.0f) };
		int vertZ{ static_cast<int>(position.z + width / 2.0f) };
		return vertices[(vertX * width + vertZ) * 3 + 1];
	}

	inline int randGen(int min, int max) {
		// static as they don't change
		static std::random_device rd;
		static std::mt19937 gen(rd());
		// We might need different ranges so now this is non-static
		std::uniform_int_distribution<int> dist(min, max);
		return dist(gen);
	}

	// Generates a random position (x, y, z) on the terrain
	inline glm::vec3 randomPosition(const std::vector<float>& vertices, int width, int height) {
		glm::vec3 pos(randGen(-height / 2, height / 2), 0.0f, randGen(-width / 2, width / 2));
		pos.y = terrainHeight(vertices, width, height, pos);
		return pos;
	}
};

#endif // !TERRAINUTILS_H
