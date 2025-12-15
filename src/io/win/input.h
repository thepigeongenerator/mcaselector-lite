/* Copyright (c) 2025 Quinn.
 * This is a file from the project MCA-Selector-Lite and is
 * licensed under the MIT Licence. See the project's LICENSE file for details. */
#pragma once

#include <GLFW/glfw3.h>

/* Handles incoming key inputs for `win`.
 * Intended to be given as an argument to `glfwSetKeyCallback`. */
void input_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
