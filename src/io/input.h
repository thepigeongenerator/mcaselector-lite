/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

#include <GLFW/glfw3.h>

/* Handles incoming key inputs for `win`.
 * Intended to be given as an argument to `glfwSetKeyCallback`. */
void input_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
