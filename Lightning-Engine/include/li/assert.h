#ifndef LI_ASSERT_H
#define LI_ASSERT_H

#define li_assert(condition) li_assert_impl(condition, __FILE__, __LINE__)

void li_assert_impl(int condition, const char* file, int line);

#endif
