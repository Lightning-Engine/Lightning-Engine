#ifndef LI_DL_DL_H
#define LI_DL_DL_H

/**
 * @file
 * @brief Dynamic library loading.
 * @note This interface is not Thread-Safe
 *
 * @code
 * dl = li_dl_open("libc.so.6");
 * if (dl == NULL) {
 *     dl = li_dl_open("msvcrt.dll");
 * }
 * if (dl == NULL) {
 *     dl = li_dl_open("libSystem.dylib");
 * }
 * if (dl == NULL) {
 *     printf("%s\n", li_dl_error());
 * } else {
 *     puts = (void (*)(const char *)) li_dl_fun(dl, "puts");
 *     if (puts == NULL) {
 *         printf("%s\n", li_dl_error());
 *     } else {
 *         puts("Hello, world!");
 *     }
 *     li_dl_close(dl);
 * }
 * @endcode
 */

typedef struct li_dl *li_dl_t;
typedef void         *li_dl_sym_t;
typedef void (*li_dl_fun_t)(void);

struct li_dl_impl {
    void (*close)(li_dl_t dl);
    li_dl_sym_t (*sym)(li_dl_t dl, const char *name);
    li_dl_fun_t (*fun)(li_dl_t dl, const char *name);
};

struct li_dl {
    const struct li_dl_impl *impl;
};

extern const char *li_dl_error_str;

/**
 * @brief Open a dynamic library.
 * @param name The name of the library to open, or NULL for the main program.
 * @return A handle to the library, or NULL on failure.
 */
li_dl_t     li_dl_open(const char *name);
/**
 * @brief Close a dynamic library.
 * @param dl The library to close.
 */
void        li_dl_close(li_dl_t dl);
/**
 * @brief Get a symbol from a dynamic library.
 * @param dl The library to get the symbol from.
 * @param name The name of the symbol to get.
 * @return The symbol, or NULL on failure.
 */
li_dl_sym_t li_dl_sym(li_dl_t dl, const char *name);
/**
 * @brief Get a function from a dynamic library.
 * @param dl The library to get the function from.
 * @param name The name of the function to get.
 * @return The function, or NULL on failure.
 */
li_dl_fun_t li_dl_fun(li_dl_t dl, const char *name);
/**
 * @brief Get the error message from the last failed operation.
 * @return The error message.
 */
const char *li_dl_error(void);

#endif
