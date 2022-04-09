#ifndef LI_DL_H
#define LI_DL_H

/**
 * @file dl.h
 * @brief Dynamic library loading.
 */

/**
 * @brief Load a dynamic library.
 * @param filename The name of the library to load.
 * @return A handle to the library, or NULL on failure.
 * @note The returned handle must be closed with li_dlclose().
 */
void *li_dlopen(const char *filename);
/**
 * @brief Close a dynamic library.
 * @param handle The handle to the library to close.
 * @return 0 on success, or non-zero on failure.
 */
int   li_dlclose(void *handle);
/**
 * @brief Get a symbol from a dynamic library.
 * @param handle The handle to the library.
 * @param symbol The name of the symbol to get.
 * @return A pointer to the symbol, or NULL on failure.
 */
void *li_dlsym(void *handle, const char *symbol);

#endif
