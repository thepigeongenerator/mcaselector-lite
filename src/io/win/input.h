/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#pragma once

#include <GLFW/glfw3.h>

/* Handles incoming key inputs for `win`.
 * Intended to be given as an argument to `glfwSetKeyCallback`. */
void input_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
