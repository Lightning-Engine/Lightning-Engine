#ifndef LI_STD_H
#define LI_STD_H

#include <stddef.h>

/**
 * @file
 * @brief Standard library wrappers.
 */

void *li_std_malloc(size_t size);
void *li_std_realloc(void *ptr, size_t size);
void  li_std_free(void *ptr);

char *li_std_strcpy(char *dest, const char *src);
char *li_std_strncpy(char *dest, const char *src, size_t count);
char *li_std_strcat(char *dest, const char *src);
char *li_std_strncat(char *dest, const char *src, size_t count);

size_t li_std_strlen(const char *str);
int    li_std_strcmp(const char *lhs, const char *rhs);
int    li_std_strncmp(const char *lhs, const char *rhs, size_t count);
char  *li_std_strchr(const char *str, int ch);
char  *li_std_strrchr(const char *str, int ch);
size_t li_std_strspn(const char *dest, const char *accept);
size_t li_std_strcspn(const char *dest, const char *reject);
char  *li_std_strpbrk(const char *dest, const char *breakset);
char  *li_std_strstr(char *dest, const char *substr);
char  *li_std_strtok(char *str, const char *delim);

void *li_std_memchr(const void *ptr, int ch, size_t count);
int   li_std_memcmp(const void *lhs, const void *rhs, size_t count);
void *li_std_memset(void *dest, int ch, size_t count);
void *li_std_memcpy(void *dest, const void *src, size_t count);
void *li_std_memmove(void *dest, const void *src, size_t count);

#endif
