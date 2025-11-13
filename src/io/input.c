/* Copyright (c) 2025 Quinn.
 * This is a file from the project MCA-Selector-Lite and is
 * licensed under the MIT Licence. See the project's LICENSE file for details. */
#include "input.h"

#include <GLFW/glfw3.h>

void input_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	(void)win, (void)key, (void)scancode, (void)action, (void)mods; // make the compiler shut up as this is fine
#ifndef NDEBUG
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, 1);
#endif
}
