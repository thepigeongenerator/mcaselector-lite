// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "window.h"

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
	// initialize the window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	win = glfwCreateWindow(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, WIN_NAME, NULL, NULL);
	if (!win) return 1;

	// configure callbacks
	glfwSetKeyCallback(win, key_callback);
	glfwSwapInterval(1); // wait 1 screen update for a redraw a.k.a. "vsync". (not really applicable in this case but eh)

	// setup OpenGL
	glfwMakeContextCurrent(win);
	if (!gladLoadGL(glfwGetProcAddress)) return 1;

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
