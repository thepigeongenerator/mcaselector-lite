/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

#include <GLFW/glfw3.h>
#include <glad/gl.h>

int  render_init(void);
void render_update(GLFWwindow *win);
void render_free(void);
