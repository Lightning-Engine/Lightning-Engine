#ifndef LI_DL_H
#define LI_DL_H

/**
 * @file li_dl.h
 * @brief Dynamic library loader.
 *
 * @code
 * li_dl_t *handle;
 * void (*puts)(const char *);
 * if (!li_dl_init()) {
 *     if ((handle = li_dl_open("libc.so.6"))) {
 *         if ((puts = (void (*)(const char *)) li_dl_fsym(handle, "puts"))) {
 *             puts("Hello, World!");
 *         }
 *         li_dl_close(handle);
 *     }
 * }
 * @endcode
 */

typedef struct li_dl li_dl_t;
typedef void (*li_fn_t)(void);

/**
 * @brief Initialize the dynamic library loader.
 * @return 0 on success, -1 on failure.
 */
int      li_dl_init(void);
/**
 * @brief Load a dynamic library.
 * @param filename The filename of the library to load.
 * @return The handle to the loaded library, or NULL on failure.
 */
li_dl_t *li_dl_open(const char *filename);
/**
 * @brief Unload a dynamic library.
 * @param handle The handle to the library.
 * @return 0 on success, -1 on failure.
 */
int      li_dl_close(li_dl_t *handle);
/**
 * @brief Get a pointer from a dynamic library.
 * @param handle The handle to the library.
 * @param symbol The name of the pointer to get.
 * @return The pointer, or NULL on failure.
 */
void    *li_dl_psym(li_dl_t *handle, const char *symbol);
/**
 * @brief Get a function pointer from a dynamic library.
 * @param handle The handle to the library.
 * @param symbol The name of the function to get.
 * @return The function pointer, or NULL on failure.
 */
li_fn_t  li_dl_fsym(li_dl_t *handle, const char *symbol);

#endif
