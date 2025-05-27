#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "shader.h"
#include "skybox.h"
#include "terrain.h"

class Game {
public:
	static constexpr int WIDTH{ 1920 };
	static constexpr int HEIGHT{ 1080 };
	Camera& camera;

	Game(Camera& camera);
	~Game();

	int init(bool fullscreen);
	int run();

private:
	float deltaTime{};
	float lastFrame{};

	GLFWwindow* window;

	// Easily draw/render the objects
	std::vector<std::shared_ptr<GameObject>> drawables;
	std::unique_ptr<SkyBox> skyBox;
	std::shared_ptr<Terrain> terrain;

	int initGlfw(bool fullscreen);
	void initEnvironment();

	void render();
	void processInput();

	// void update();

};


#endif // !GAME_H
