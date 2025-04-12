#include "error.h"

int main(int argc, char** argv) {
	(void)argc, (void)argv;
	debug("%s", "owo");
	info("%s", "owo");
	warn("%s", "owo");
	error("%s", "owo");
	fatal("%s", "owo");
}
