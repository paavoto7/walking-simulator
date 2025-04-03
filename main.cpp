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

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
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

	constexpr array<glm::vec3, 10> cubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

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

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;

	unsigned char* heightMapData{ stbi_load("assets/heightMap.png", &width, &height, &nrChannels, 0) };

	if (heightMapData)
	{
		std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	vector<float> vertices;

	const float yScale{ 64.0f / 256.0f };
	const float yShift{ 16.0f };
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			unsigned char* texel{ heightMapData + (width * i + j) * nrChannels };

			unsigned char y = *texel; // Just dereference i.e. *texel

			vertices.push_back(-height / 2.0f + i); // x
			vertices.push_back(static_cast<int>(y) * yScale - yShift); // y
			vertices.push_back(-width / 2.0f + j); // z
		}
	}

	std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;

	stbi_image_free(heightMapData);

	vector<unsigned> indices;

	const unsigned NUM_STRIPS = height - 1;
	const unsigned NUM_VERTS_PER_STRIP = width * 2;

	for (unsigned i = 0; i < NUM_STRIPS; ++i) {
		for (unsigned j = 0; j < width; ++j) {
			for (unsigned k = 0; k < 2; ++k) {
				indices.push_back(j + width * (i + k));
			}
		}
	}

	// Here starts the heightmap terrain
	GLuint groundVBO, groundVAO, groundEBO;
	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glGenBuffers(1, &groundEBO);

	glBindVertexArray(groundVAO);

	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)* indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	// Ends here

	

	unsigned int texture1, texture2;

	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texWidth, texHeight, texNrChannels;
	unsigned char* data{ stbi_load("assets/container.jpg", &texWidth, &texHeight, &texNrChannels, 0) };

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture!" << endl;
	}

	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("assets/awesomeface.png", &texWidth, &texHeight, &texNrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture!" << endl;
	}

	stbi_image_free(data);

	shaderProgram.use();
	shaderProgram.setInt("texture1", 0);
	shaderProgram.setInt("texture2", 1);
	shaderProgram.setFloat("visibility", visibility);
	glEnable(GL_DEPTH_TEST);

	terrainHeight(vertices, width, height);
	camera.Position.y = camera.terrainLevel + camera.characterHeight;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame{ static_cast<float>(glfwGetTime()) };
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glm::mat4 view{ camera.GetViewMatrix() };
		glm::mat4 projection{ glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 1000.0f)};
		
		shaderProgram.use();
		glBindVertexArray(VAO);
		
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setFloat("visibility", visibility);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glm::mat4 model{ glm::translate(glm::mat4(1.0f), cubePositions[1]) };
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		shaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		model = glm::translate(glm::mat4(1.0f), cubePositions[0]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, -2.0f));
		shaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		*/

		terrainHeight(vertices, width, height);

		terrainShaderProgram.use();
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		glBindVertexArray(groundVAO);
		model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
		terrainShaderProgram.setMat4("model", model);
		for (unsigned strip = 0; strip < NUM_STRIPS; ++strip) {
			glDrawElements(
				GL_TRIANGLE_STRIP,
				NUM_VERTS_PER_STRIP,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned) * NUM_VERTS_PER_STRIP * strip)
			);
		}

		/*
		for (int i = 0; i < cubePositions.size(); ++i) {
			glm::mat4 model{ glm::translate(glm::mat4(1.0f), cubePositions[i]) };
			model = glm::rotate(model, glm::radians(20.0f * (i + 1)) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgram.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		*/
		camera.update(deltaTime);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &groundVAO);
	glDeleteBuffers(1, VBO);
	glDeleteBuffers(1, &groundVBO);
	glDeleteBuffers(1, &groundEBO);
	glDeleteProgram(shaderProgram.ID);

	cout << "Reached the end" << endl;

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	static constexpr float change{ 0.001f };
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		visibility = visibility + change <= 1.0 ? visibility + change : 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		visibility = visibility - change >= 0 ? visibility - change : 0.0f;
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
