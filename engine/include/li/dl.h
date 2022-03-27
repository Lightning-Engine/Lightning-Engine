#ifndef LI_DL_H
#define LI_DL_H

#if defined LI_WIN32
/* Special case for Windows since they are special */
# include <windows.h>

typedef HMODULE _li_dl_t;
#else
/* If its not windows, it's either macOS or Linux which both use void* */
# include <dlfcn.h>

typedef void *_li_dl_t;
#endif

/* Type for storing a reference to a dynamic library */
typedef _li_dl_t li_dl_t;

/* Open a dynamic library. Returns 0 on success */
int   li_dl_open(li_dl_t *dl, const char *path);
/* Close a dynamic library. Returns 0 on success */
int   li_dl_close(li_dl_t *dl);
/* Load a symbol from a dynamic library. Returns null pointer on failure */
void *li_dl_sym(li_dl_t *dl, const char *name);

#endif
