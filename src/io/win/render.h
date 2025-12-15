/* Copyright (c) 2025 Quinn.
 * This is a file from the project MCA-Selector-Lite and is
 * licensed under the MIT Licence. See the project's LICENSE file for details. */
#pragma once

#include <GLFW/glfw3.h>
#include <glad/gl.h>

int  render_init(void);
void render_update(GLFWwindow *win);
void render_free(void);
