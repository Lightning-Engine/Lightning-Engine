#include "li/entry.h"

int li_argc;
char **li_argv;

int main(int argc, char **argv) {
	li_argc = argc;
	li_argv = argv;
	return li_main();
}
