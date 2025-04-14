// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

#include <GLFW/glfw3.h>

struct renderdat {
	GLFWwindow* win;
};

int window_init(struct renderdat* restrict rdat);
void window_free(struct renderdat* restrict rdat);
