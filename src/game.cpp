#include "game.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resource_manager.h"
#include "terrainUtils.h"
#include "model.h"
#include "game_object.h"
#include "water.h"
#include "instancedModel.h"
#include "callbacks.h"

Game::Game(Camera& camera) : camera(camera) {}

Game::~Game() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

// Init function is needed to initialise GLFW before other things
int Game::init(bool fullscreen) {
	int success = 0;
	success = initGlfw(fullscreen);
	if (success == 0) {
		initEnvironment();
	}
	return success;
	
}

int Game::run() {
	Shader& shaderProgram = ResourceManager::GetShader("shader");
	Shader& instancedShaderProgram = ResourceManager::GetShader("instancedShader");
	Shader& terrainShaderProgram = ResourceManager::GetShader("terrainShader");
	Shader& skyboxShaderProgram = ResourceManager::GetShader("skyboxShader");

	while (!glfwWindowShouldClose(window)) {
		float currentFrame{ static_cast<float>(glfwGetTime()) };
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput();

		glClearColor(0.5f, 0.5f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view{ camera.GetViewMatrix() };
		glm::mat4 projection{
			glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f)
		};
		glm::mat4 model(1.0f);

		// Planning on changing the matrix sets elsewhere
		shaderProgram.use();
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		instancedShaderProgram.use();
		instancedShaderProgram.setMat4("view", view);
		instancedShaderProgram.setMat4("projection", projection);

		terrainShaderProgram.use();
		terrainShaderProgram.setMat4("view", view);
		terrainShaderProgram.setMat4("projection", projection);
		terrainShaderProgram.setMat4("model", model);

		skyboxShaderProgram.use();
		skyboxShaderProgram.setMat4("projection", projection);
		// Remove the translation effect so that the skybox is always around camera/player
		skyboxShaderProgram.setMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));

		render();

		camera.update(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "Game loop finished" << std::endl;
	return 0;
}

void Game::render() {
	camera.terrainLevel = TerrainUtils::terrainHeight(terrain->vertices, terrain->width, terrain->height, camera.Position);

	// I may have to rethink this at some point
	for (const auto& game_object : drawables) {
		game_object->draw();
	}

	skyBox->draw();
}

int Game::initGlfw(bool fullscreen) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if (fullscreen) {
		// Doing this makes the the fullscreen switch a lot faster
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		window = glfwCreateWindow(mode->width, mode->height, "Walking simulator", monitor, nullptr);
	} else {
		window = glfwCreateWindow(WIDTH, HEIGHT, "Walking simulator", nullptr, nullptr);
	}

	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, &camera);
	glfwSetFramebufferSizeCallback(window, Callbacks::framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Callbacks::mouse_callback);
	glfwSetScrollCallback(window, Callbacks::scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return 1;
	}

	return 0;
}
void Game::initEnvironment() {

	ResourceManager::LoadShader("shader", "./shaders/vertex.glsl", "./shaders/fragment.glsl");
	auto& terrainShader = ResourceManager::LoadShader("terrainShader", "./shaders/terrainVertex.glsl", "./shaders/terrainFragment.glsl");
	auto& instancedShaderProgram = ResourceManager::LoadShader("instancedShader", "./shaders/instancedVertex.glsl", "./shaders/fragment.glsl");
	auto& skyboxShaderProgram = ResourceManager::LoadShader("skyboxShader", "./shaders/skyboxVertex.glsl", "./shaders/skyboxFragment.glsl");

	skyBox = std::make_unique<SkyBox>(skyboxShaderProgram);

	terrain = std::make_shared<Terrain>(terrainShader);
	drawables.push_back(terrain);

	const float halfWidth{ terrain->width / 2.0f };
	const float halfHeight{ terrain->height / 2.0f };

	// First version of static and plain water
	std::vector<float> waterVertices = {
		// First Triangle
		-halfWidth, 0.0f, -halfHeight,  0.0f, 0.0f,
		 halfWidth, 0.0f, -halfHeight,  1.0f, 0.0f,
		 halfWidth, 0.0f,  halfHeight,  1.0f, 1.0f,

		 // Second Triangle
		 -halfWidth, 0.0f, -halfHeight,  0.0f, 0.0f,
		  halfWidth, 0.0f,  halfHeight,  1.0f, 1.0f,
		 -halfWidth, 0.0f,  halfHeight,  0.0f, 1.0f
	};

	drawables.push_back(std::make_shared<Water>(waterVertices, terrainShader));

	auto& flower = ResourceManager::LoadModel("flower", "assets/Flower/Flower/Flower.obj", "instancedShader");
	flower.height *= 0.15f;

	drawables.push_back(std::make_shared<InstancedModel>(instancedShaderProgram, flower, terrain, 1000));

	skyboxShaderProgram.use();
	skyboxShaderProgram.setInt("skybox", 0);

	glEnable(GL_DEPTH_TEST);
	// Allow transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.terrainLevel = TerrainUtils::terrainHeight(terrain->vertices, terrain->width, terrain->height, camera.Position);
	camera.Position.y = camera.terrainLevel + camera.characterHeight;
}

void Game::processInput() {
	static constexpr float change{ 0.001f };
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.ProcessKeyboard(SPACE, deltaTime);
	}
}
