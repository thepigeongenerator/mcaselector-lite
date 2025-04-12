#include "GLFW/glfw3.h"
#include "error.h"
#include "window/render.h"

struct renderdat rdat; // contains the relevant data needed for rendering, contains rubbish data until init was

static inline int init(void) {
	glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE); // disable joystick buttons

	if (!glfwInit()) return 1;        // initialize GLFW
	if (render_init(&rdat)) return 1; // initialize the rendering and create a window

	return 0;
}

static inline void quit(void) {
	glfwTerminate();
}

int main(int argc, char** argv) {
	(void)argc, (void)argv;
	if (init()) fatal("failed to initialize!");
	while (1);
	quit();
}
