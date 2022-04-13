#ifndef LI_DL_H
#define LI_DL_H

/**
 * @file
 * @brief Dynamic library loading.
 * @code
 * dl = li_dl_open("libc.so.6");
 * if (dl == NULL) {
 *     dl = li_dl_open("msvcrt.dll");
 * }
 * if (dl == NULL) {
 *     dl = li_dl_open("libSystem.dylib");
 * }
 * if (dl != NULL) {
 *     puts = (void (*)(const char *)) li_dl_fun(dl, "puts");
 *     if (puts != NULL) {
 *         puts("Hello, World!");
 *     }
 *     li_dl_close(dl);
 * }
 * @endcode
 */

typedef void *li_dl_t;
typedef void *li_dl_sym_t;
typedef void (*li_dl_fun_t)(void);

/**
 * @brief Open a dynamic library.
 * @param name The name of the library to open.
 * @return A handle to the library, or NULL on failure.
 */
li_dl_t     li_dl_open(const char *name);
/**
 * @brief Close a dynamic library.
 * @param dl The library to close.
 * @return 0 on success, -1 on failure.
 */
int         li_dl_close(li_dl_t dl);
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

#endif
