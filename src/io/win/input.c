/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
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
