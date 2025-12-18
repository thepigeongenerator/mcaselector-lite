/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#pragma once

#include <GLFW/glfw3.h>
#include <glad/gl.h>

int  render_init(void);
void render_update(GLFWwindow *win);
void render_free(void);
