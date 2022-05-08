#include "li/dl/dyld.h"

#include "li/std.h"

const struct li_dl_impl li_dl_dyld_impl = { li_dl_dyld_open, li_dl_dyld_close,
                                            li_dl_dyld_sym, li_dl_dyld_fun };

li_dl_t li_dl_dyld_open(const char *name) {
    struct li_dl_dyld          *dl_dyld;
    NSObjectFileImage           image;
    NSObjectFileImageReturnCode result;
    dl_dyld = li_std_malloc(sizeof *dl_dyld);
    if (dl_dyld != NULL) {
        dl_dyld->base.impl = &li_dl_dyld_impl;
        dl_dyld->module    = NULL;
        dl_dyld->image     = NULL;
        if (name == NULL) {
            return (li_dl_t) dl_dyld;
        }
        result = NSCreateObjectFileImageFromFile(name, &image);
        if (result == NSObjectFileImageSuccess) {
            dl_dyld->module =
                NSLinkModule(image, name, NSLINKMODULE_OPTION_RETURN_ON_ERROR);
            NSDestroyObjectFileImage(image);
        } else {
            dl_dyld->image = NSAddImage(
                name, NSADDIMAGE_OPTION_RETURN_ON_ERROR
                          | NSADDIMAGE_OPTION_WITH_SEARCHING);
        }
        if (dl_dyld->module != NULL || dl_dyld->image != NULL) {
            return (li_dl_t) dl_dyld;
        }
        li_dl_dyld_error();
        li_std_free(dl_dyld);
    }
    return NULL;
}

void li_dl_dyld_close(li_dl_t dl) {
    struct li_dl_dyld *dl_dyld = (struct li_dl_dyld *) dl;
    if (dl_dyld->module != NULL) {
        NSUnLinkModule(dl_dyld->module, NSUNLINKMODULE_OPTION_NONE);
    }
    li_std_free(dl_dyld);
}

li_dl_sym_t li_dl_dyld_sym(li_dl_t dl, const char *name) {
    struct li_dl_dyld *dl_dyld = (struct li_dl_dyld *) dl;
    NSSymbol           symbol;
    char              *symbol_name;
    symbol_name = li_std_malloc(li_std_strlen(name) + 2);
    if (symbol_name != NULL) {
        symbol_name[0] = '_';
        li_std_strcpy(symbol_name + 1, name);
        if (dl_dyld->module != NULL) {
            symbol = NSLookupSymbolInModule(dl_dyld->module, symbol_name);
        } else if (dl_dyld->image != NULL) {
            symbol = NSLookupSymbolInImage(
                dl_dyld->image, symbol_name,
                NSLOOKUPSYMBOLINIMAGE_OPTION_BIND
                    | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR);
        } else {
            symbol = NSLookupAndBindSymbol(symbol_name);
        }
        li_std_free(symbol_name);
        if (symbol != NULL) {
            return NSAddressOfSymbol(symbol);
        }
        li_dl_dyld_error();
    }
    return NULL;
}

li_dl_fun_t li_dl_dyld_fun(li_dl_t dl, const char *name) {
    union li_dl_symbol symbol;
    symbol.sym = li_dl_dyld_sym(dl, name);
    return symbol.fun;
}

void li_dl_dyld_error(void) {
    NSLinkEditErrors error;
    int              errnum;
    const char      *errfile;
    const char      *errstr;
    NSLinkEditError(&error, &errnum, &errfile, &errstr);
    li_dl_set_error(errstr);
}
