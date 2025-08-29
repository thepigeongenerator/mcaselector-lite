/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "io/window.h"


#define WIN_NAME           "MCA Selector Lite"
#define WIN_DEFAULT_WIDTH  640
#define WIN_DEFAULT_HEIGHT 480

// callback for GLFW errors
static void error_callback(int err, const char *const msg) {
	fprintf(stderr, "\033[91mE: glfw returned (%i); \"%s\"\033[0m\n", err, msg);
}

static inline int init(void) {
	glfwSetErrorCallback(error_callback);
	glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE); // disable joystick buttons

	if (!glfwInit()) return 1; // initialize GLFW
	if (window_init()) return 1;

	return 0;
}

static inline void quit(void) {
	glfwTerminate();
}

int main(int argc, char **argv) {
	(void)argc, (void)argv;
	printf("debug: [DBG], info: [INF], warning: [WAR], error: [ERR], fatal: [FAT]\n");
	if (init()) fatal("failed to initialize!");

	window_loop();
	quit();

	// return success, since some architectures do not follow 0=success
	return EXIT_SUCCESS;
}
