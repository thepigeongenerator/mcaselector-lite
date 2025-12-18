/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#pragma once

#include <glad/gl.h>
#include <stddef.h>
#include <stdint.h>

/* Initialises the (embedded) shaders onto `pipe` */
int shader_init(GLuint pipe);
