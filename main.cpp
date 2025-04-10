#include <array>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "terrain.h"
#include "texture.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

inline void terrainHeight(const vector<float>& vertices, float width, float height);

constexpr int WIDTH{ 1920 };
constexpr int HEIGHT{ 1080 };

//Camera camera(glm::vec3(0.0f, 100.0f, 3.0f));
Camera camera(glm::vec3(67.0f, 227.5f, 169.9f));

float lastX{ WIDTH / 2 };
float lastY{ HEIGHT / 2 };
bool firstMouse{ true };

float deltaTime{};
float lastFrame{};

float visibility{ 0.2f };

constexpr float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float groundVertices[] = {
	// First Triangle
	-1.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 1.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  0.0f,  1.0f,  1.0f, 1.0f,

	 // Second Triangle
	 -1.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	  1.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	 -1.0f,  0.0f,  1.0f,  0.0f, 1.0f
};

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Walking simulator", nullptr, nullptr);
	if (window == nullptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialise GLAD" << endl;
		return -1;
	}

	Shader shaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	Shader terrainShaderProgram("./shaders/terrainVertex.glsl", "./shaders/terrainFragment.glsl");

	GLuint VBO[2], VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	Terrain terrain;
	terrain.init();

	Texture texture1("assets/container.jpg");
	Texture texture2("assets/awesomeface.png");

	shaderProgram.use();
	shaderProgram.setInt("texture1", 0);
	shaderProgram.setInt("texture2", 1);
	shaderProgram.setFloat("visibility", visibility);
	glEnable(GL_DEPTH_TEST);

	terrainHeight(terrain.vertices, terrain.width, terrain.height);
	camera.Position.y = camera.terrainLevel + camera.characterHeight;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame{ static_cast<float>(glfwGetTime()) };
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture1.bind(GL_TEXTURE0);
		texture2.bind(GL_TEXTURE1);

		glm::mat4 view{ camera.GetViewMatrix() };
		glm::mat4 projection{ glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 1000.0f)};
		
		shaderProgram.use();
		glBindVertexArray(VAO);
		
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setFloat("visibility", visibility);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glm::mat4 model{ glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,  0.0f,  0.0f)) };
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		shaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		terrainHeight(terrain.vertices, terrain.width, terrain.height);

		terrainShaderProgram.use();
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
		terrainShaderProgram.setMat4("model", model);

		terrain.draw();

		camera.update(deltaTime);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, VBO);
	
	glDeleteProgram(shaderProgram.ID);
	glDeleteProgram(terrainShaderProgram.ID);

	cout << "Reached the end" << endl;

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
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
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset{ static_cast<float>(xpos - lastX) };
	float yoffset{ static_cast<float>(lastY - ypos) };
	lastX = xpos;
	lastY = ypos;

	static constexpr float sensitivity{ 0.2f };
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

inline void terrainHeight(const vector<float>& vertices, float width, float height) {
	glm::vec3 camPos{ camera.Position };
	int vertX{ static_cast<int>(camPos.x + height / 2.0f) };
	int vertZ{ static_cast<int>(camPos.z + width / 2.0f) };
	camera.terrainLevel = vertices[(vertX * width + vertZ) * 3 + 1];
}
