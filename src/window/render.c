#include "render.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>

// macros for ease of access
#define WIN_NAME           "MCA Selector Lite"
#define WIN_DEFAULT_WIDTH  640
#define WIN_DEFAULT_HEIGHT 480

int render_init(struct renderdat* restrict rdat) {
	GLFWwindow* const win = glfwCreateWindow(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, WIN_NAME, NULL, NULL);
	if (win == NULL) return 1;
	rdat->win = win;

	return 0;
}

void render_free(struct renderdat* restrict rdat) {
	glfwDestroyWindow(rdat->win);
}
