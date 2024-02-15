#include "../assert.h"
#include "../system-interface.h"

void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function)
{
    sysi_assert_fail(__assertion, __file, __line, __function);
}