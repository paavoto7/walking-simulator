#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "terrainUtils.h"
#include "shader.h"
#include "camera.h"
#include "terrain.h"
#include "texture.h"
#include "model.h"
#include "game_object.h"
#include "water.h"
#include "skybox.h"
#include "instancedModel.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

inline void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

constexpr int WIDTH{ 1920 };
constexpr int HEIGHT{ 1080 };
const bool fullscreen{ false };

//Camera camera(glm::vec3(0.0f, 100.0f, 3.0f));
Camera camera(glm::vec3(67.0f, 227.5f, 169.9f));

float lastX{ WIDTH / 2 };
float lastY{ HEIGHT / 2 };
bool firstMouse{ true };

float deltaTime{};
float lastFrame{};

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	GLFWwindow* window;
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
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialise GLAD" << endl;
		return 1;
	}

	Shader shaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	Shader terrainShaderProgram("./shaders/terrainVertex.glsl", "./shaders/terrainFragment.glsl");
	Shader skyboxShaderProgram("./shaders/skyboxVertex.glsl", "./shaders/skyboxFragment.glsl");
	Shader instancedShaderProgram("./shaders/instancedVertex.glsl", "./shaders/fragment.glsl");
	cout << "Shaders initialised" << endl;

	// Easily draw/render the objects
	vector<shared_ptr<GameObject>> drawables;

	shared_ptr<Terrain> terrain = make_shared<Terrain>();
	drawables.push_back(terrain);

	const float halfWidth{ terrain->width / 2.0f };
	const float halfHeight{ terrain->height / 2.0f };

	// First version of static and plain water
	const std::vector<float> waterVertices = {
		// First Triangle
		-halfWidth, 0.0f, -halfHeight,  0.0f, 0.0f,
		 halfWidth, 0.0f, -halfHeight,  1.0f, 0.0f,
		 halfWidth, 0.0f,  halfHeight,  1.0f, 1.0f,

		 // Second Triangle
		 -halfWidth, 0.0f, -halfHeight,  0.0f, 0.0f,
		  halfWidth, 0.0f,  halfHeight,  1.0f, 1.0f,
		 -halfWidth, 0.0f,  halfHeight,  0.0f, 1.0f
	};

	shared_ptr<Water> water = make_shared<Water>(waterVertices);
	drawables.push_back(water);

	SkyBox skyBox(skyboxShaderProgram, "assets/gloomySkybox/", "png");

	Model flower("assets/Flower/Flower/Flower.obj", instancedShaderProgram);
	flower.height *= 0.2f;

	InstancedModel flowerField(instancedShaderProgram, flower, terrain, 1000);

	skyboxShaderProgram.use();
	skyboxShaderProgram.setInt("skybox", 0);

	glEnable(GL_DEPTH_TEST);
	// Allow transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.terrainLevel = TerrainUtils::terrainHeight(terrain->vertices, terrain->width, terrain->height, camera.Position);
	camera.Position.y = camera.terrainLevel + camera.characterHeight;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame{ static_cast<float>(glfwGetTime()) };
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.5f, 0.5f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view{ camera.GetViewMatrix() };
		glm::mat4 projection{ glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 1000.0f)};
		glm::mat4 model(1.0f);
		
		camera.terrainLevel = TerrainUtils::terrainHeight(terrain->vertices, terrain->width, terrain->height, camera.Position);

		// Rendering the flower here for now
		shaderProgram.use();
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		model = glm::mat4(1.0f);

		terrainShaderProgram.use();
		terrainShaderProgram.setMat4("view", view);
		terrainShaderProgram.setMat4("projection", projection);
		//model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
		terrainShaderProgram.setMat4("model", model);

		// I may have to rethink this when more shaders accumulate...
		for (const auto& game_object: drawables) {
			game_object->draw();
		}

		// Draw the flowers
		instancedShaderProgram.use();
		instancedShaderProgram.setMat4("view", view);
		instancedShaderProgram.setMat4("projection", projection);
		flowerField.draw();

		skyboxShaderProgram.use();
		skyboxShaderProgram.setMat4("projection", projection);
		// Remove the translation effect so that the skybox is always around camera/player
		skyboxShaderProgram.setMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));

		skyBox.draw();

		camera.update(deltaTime);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cout << "Reached the end" << endl;

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

inline void processInput(GLFWwindow* window) {
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}
	float xoffset{ static_cast<float>(xpos - lastX) };
	float yoffset{ static_cast<float>(lastY - ypos) };
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	static constexpr float sensitivity{ 0.2f };
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
