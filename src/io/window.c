// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "window.h"

#include <stdio.h>

#include "../error.h"

// include before GLFW
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

// include system libraries
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdlib.h>

#include "input.h"

// macros for ease of access
#define WIN_NAME           "MCA Selector Lite"
#define WIN_DEFAULT_WIDTH  640
#define WIN_DEFAULT_HEIGHT 480

static GLFWwindow* win = NULL;

int window_init(void) {
#ifndef NDEBUG
	glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif

	// initialize the window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // sets the profile to "core", so old, deprecated functions are disabled.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	win = glfwCreateWindow(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, WIN_NAME, NULL, NULL);
	if (!win) return 1;

	// setup OpenGL
	glfwMakeContextCurrent(win);
	if (!gladLoadGL(glfwGetProcAddress)) return 1;

	// configure callbacks
	glfwSetKeyCallback(win, key_callback);
	glfwSwapInterval(1); // wait 1 screen update for a redraw a.k.a. "vsync". (not really applicable in this case but eh)

	// print the OpenGL version information
	debug("version info:");
	debug("\tvendor:       %s", glGetString(GL_VENDOR));
	debug("\trenderer:     %s", glGetString(GL_RENDERER));
	debug("\tversion:      %s", glGetString(GL_VERSION));
	debug("\tshading lang: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	return 0;
}

void window_loop(void) {
	assert(win != NULL);

	while (!glfwWindowShouldClose(win)) {
		glfwWaitEvents(); // wait till an update has been given
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(win);
	}

	glfwDestroyWindow(win);
}
