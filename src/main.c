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

/* reroutes GLFW errors to our logging system. */
static void error_callback(int err, const char *const msg) {
	error("glfw returned (%i); \"%s\"", err, msg);
}

static void quit(void) {
	glfwTerminate();
}


/* Entry-point of the application. */
int main(int argc, char **argv) {
	(void)argc, (void)argv;
	printf("debug: [DBG], info: [INF], warning: [WAR], error: [ERR], fatal: [FAT]\n");
	atexit(quit);

	glfwSetErrorCallback(error_callback);
	glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE); // disable joystick buttons; since we won't need them
	if (!glfwInit() || window_init())
		fatal("failed to initialise!");

	window_loop();
	quit();

	// return success, since some architectures do not follow 0=success
	return EXIT_SUCCESS;
}
