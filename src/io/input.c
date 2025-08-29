/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#include "input.h"

#include <GLFW/glfw3.h>

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods) {
	(void)win, (void)key, (void)scancode, (void)action, (void)mods; // make the compiler shut up as this is fine
#ifndef NDEBUG
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, 1);
#endif
}
