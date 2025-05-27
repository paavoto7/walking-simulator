#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Callbacks {
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}

#endif // !CALLBACKS_H
