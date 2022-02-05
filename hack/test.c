#include <stdio.h>

int dlclose(void *lib) {
	(void) lib;
	return 0;
}