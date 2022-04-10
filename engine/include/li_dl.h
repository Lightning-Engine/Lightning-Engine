#ifndef LI_DL_H
#define LI_DL_H

/**
 * @file li_dl.h
 * @brief Dynamic library loader.
 *
 * @code
 * if (li_dl_init() == 0) {
 *     dl = li_dl_open("libc.so.6");
 *     if (dl == NULL) {
 *         dl = li_dl_open("msvcrt.dll");
 *     }
 *     if (dl != NULL) {
 *         puts = (void (*)(const char *)) li_dl_sym(dl, "puts");
 *         if (puts != NULL) {
 *             puts("Hello, World!");
 *         }
 *         li_dl_close(dl);
 *     }
 * }
 * @endcode
 */

typedef struct li_dl *li_dl_t;
typedef void         *li_dl_sym_t;
typedef void (*li_dl_fun_t)(void);

/**
 * @brief Initialize the dynamic library loader.
 * @return 0 on success, -1 on failure.
 */
int         li_dl_init(void);
/**
 * @brief Load a dynamic library.
 * @param name The name of the library.
 * @return The handle to the library, or NULL on failure.
 */
li_dl_t     li_dl_open(const char *name);
/**
 * @brief Unload a dynamic library.
 * @param dl The handle to the library.
 * @return 0 on success, -1 on failure.
 */
int         li_dl_close(li_dl_t dl);
/**
 * @brief Get a symbol from a dynamic library.
 * @param dl The handle to the library.
 * @param name The name of the symbol.
 * @return The symbol, or NULL on failure.
 */
li_dl_sym_t li_dl_sym(li_dl_t dl, const char *name);
/**
 * @brief Get a function from a dynamic library.
 * @param dl The handle to the library.
 * @param name The name of the function.
 * @return The function, or NULL on failure.
 */
li_dl_fun_t li_dl_fun(li_dl_t dl, const char *name);

#endif
