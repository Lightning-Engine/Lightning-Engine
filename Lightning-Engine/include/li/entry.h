#ifndef LI_ENTRY_H
#define LI_ENTRY_H

extern int li_argc;
extern char **li_argv;

int li_main(void);

#ifdef LI_ENTRY_IMPL
int main(int argc, char **argv) {
	li_argc = argc;
	li_argv = argv;
	return li_main();
}
#endif

#endif
