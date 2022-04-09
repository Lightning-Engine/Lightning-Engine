#include "li/dl.h"

#if LI_MACOS

# include <mach-o/dyld.h>

void *li_dlopen(const char *filename) {
    NSObjectFileImage image;
    if (NSCreateObjectFileImageFromFile(filename, &image)
        != NSObjectFileImageSuccess)
        return 0;
    NSModule module = NSLinkModule(image, filename, NSLINKMODULE_OPTION_NONE);
    NSDestroyObjectFileImage(image);
    return module;
}

int li_dlclose(void *handle) {
    return !NSUnLinkModule(handle, 0);
}

void *li_dlsym(void *handle, const char *symbol) {
    NSSymbol sym = NSLookupSymbolInModule(handle, symbol);
    return sym ? NSAddressOfSymbol(sym) : 0;
}

#elif LI_WIN32

# include <windows.h>

void *li_dlopen(const char *filename) {
    return LoadLibrary(filename);
}

int li_dlclose(void *handle) {
    return !FreeLibrary(handle);
}

void *li_dlsym(void *handle, const char *symbol) {
    FARPROC proc = GetProcAddress(handle, symbol);
    return *(void **) &proc;
}

#else

# include <dlfcn.h>

void *li_dlopen(const char *filename) {
    return dlopen(filename, RTLD_LAZY);
}

int li_dlclose(void *handle) {
    return dlclose(handle);
}

void *li_dlsym(void *handle, const char *symbol) {
    return dlsym(handle, symbol);
}

#endif
