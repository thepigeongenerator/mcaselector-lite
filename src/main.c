/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <stdio.h>
#include <stdlib.h>

#include "io/window.h"
#include "util/error.h"

/* reroutes GLFW errors to our logging system. */
static void error_callback(int err, const char *const msg) {
	error("glfw returned (%i); \"%s\"", err, msg);
}

static void quit(void) {
	window_free();

	/* terminates GLFW; destroying any
	 * remaining windows, or other resources held by GLFW. */
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

	/* return success, since some architectures do not follow 0=success
	 * This action will call `quit`. */
	return EXIT_SUCCESS;
}
