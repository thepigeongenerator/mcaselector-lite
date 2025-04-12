#include "error.h"

extern void test(void);

int main(int argc, char** argv) {
	(void)argc, (void)argv;
	test();
	debug("owo");
}
