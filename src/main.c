#include <stdio.h>

extern char const* get_str(void);

int main(int argc, char** argv) {
	(void)argc, (void)argv;
	printf("%s\n", get_str());
}
