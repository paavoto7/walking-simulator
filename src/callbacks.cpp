#include "callbacks.h"

#include "camera.h"
#include "game.h"

namespace Callbacks {
	static float lastX{ Game::WIDTH / 2 };
	static float lastY{ Game::HEIGHT / 2 };
	static bool firstMouse{ true };

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

		Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

		if (camera) {
			camera->ProcessMouseMovement(xoffset, yoffset);
		}
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
		if (camera) {
			camera->ProcessMouseScroll(static_cast<float>(yoffset));
		}
	}
}
