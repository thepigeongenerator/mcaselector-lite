/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#include "window.h"

#include <GLFW/glfw3.h>
#include <assert.h>
#include <glad/gl.h>

#include "../error.h"
#include "../util/intdef.h"
#include "input.h"
#include "render.h"

static struct GLFWwindow *win = NULL;

/* Initialises the GLFW window with some defaults,
 * then proceed to activate OpenGL on it. */
int window_init(void) {
	// initialise the window
#ifndef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	win = glfwCreateWindow(640, 480, "MCA-Selector lite", NULL, NULL);
	if (!win) return 1;

	glfwMakeContextCurrent(win);
	if (!gladLoadGL(glfwGetProcAddress)) return 1;
	glfwSwapInterval(1); // wait 1 screen update for a redraw a.k.a. "vsync". (not really applicable in this case but eh)

	glfwSetKeyCallback(win, input_callback);

	debug(
		"version info:\n"
		"\tvendor:       %s\n"
		"\trenderer:     %s\n"
		"\tversion:      %s\n"
		"\tshading lang: %s\n",
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER),
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));
	return 0;
}

void window_loop(void) {
	assert(win);
	render_init();
	while (!glfwWindowShouldClose(win)) {
		glfwWaitEvents();

		render_update(win);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}

void window_close(void) {
	assert(win);
	glfwSetWindowShouldClose(win, 1);
}

void window_free(void) {
	if (!win) {
		debug("window has already been freed.");
		return;
	}

	glfwDestroyWindow(win);
	render_free();
	win = NULL;
}
