#include "li/std.h"

#include <stdlib.h>
#include <string.h>

void *li_std_malloc(size_t size) {
    return malloc(size);
}

void *li_std_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

void li_std_free(void *ptr) {
    free(ptr);
}

char *li_std_strcpy(char *dest, const char *src) {
    return strcpy(dest, src);
}

char *li_std_strncpy(char *dest, const char *src, size_t count) {
    return strncpy(dest, src, count);
}

char *li_std_strcat(char *dest, const char *src) {
    return strcat(dest, src);
}

char *li_std_strncat(char *dest, const char *src, size_t count) {
    return strncat(dest, src, count);
}

size_t li_std_strlen(const char *str) {
    return strlen(str);
}

int li_std_strcmp(const char *lhs, const char *rhs) {
    return strcmp(lhs, rhs);
}

int li_std_strncmp(const char *lhs, const char *rhs, size_t count) {
    return strncmp(lhs, rhs, count);
}

char *li_std_strchr(const char *str, int ch) {
    return strchr(str, ch);
}

char *li_std_strrchr(const char *str, int ch) {
    return strrchr(str, ch);
}

size_t li_std_strspn(const char *dest, const char *accept) {
    return strspn(dest, accept);
}

size_t li_std_strcspn(const char *dest, const char *reject) {
    return strcspn(dest, reject);
}

char *li_std_strpbrk(const char *dest, const char *breakset) {
    return strpbrk(dest, breakset);
}

char *li_std_strstr(char *dest, const char *substr) {
    return strstr(dest, substr);
}

char *li_std_strtok(char *str, const char *delim) {
    return strtok(str, delim);
}

void *li_std_memchr(const void *ptr, int ch, size_t count) {
    return memchr(ptr, ch, count);
}

int li_std_memcmp(const void *lhs, const void *rhs, size_t count) {
    return memcmp(lhs, rhs, count);
}

void *li_std_memset(void *dest, int ch, size_t count) {
    return memset(dest, ch, count);
}

void *li_std_memcpy(void *dest, const void *src, size_t count) {
    return memcpy(dest, src, count);
}

void *li_std_memmove(void *dest, const void *src, size_t count) {
    return memmove(dest, src, count);
}
