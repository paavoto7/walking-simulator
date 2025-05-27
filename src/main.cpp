#include "game.h"

#include <iostream>

#include <glm/glm.hpp>

#include "camera.h"

int main() {
	Camera camera(glm::vec3(-234.0f, 0.0f, 346.0f));
	Game game(camera);

	int exitCode = 0;
	const bool fullscreen{ false };
	if ((exitCode = game.init(fullscreen)) != 0) {
		return exitCode;
	}
	std::cout << "Initialisation done" << std::endl;

	std::cout << "Starting game loop" << std::endl;
	exitCode = game.run();

	return exitCode;
}
