#include <slibc/system-interface.h>
#include <logger/logger.h>
#include <memory/pmm.h>

// TODO: make 'sysi_assert_failed' arch-independant
void *sysi_assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function)
{
    log(LT_ERROR, "slibc", "Assert failed! %s in %s, line %d, in '%s'\n", __assertion, __file, __line, __function);
    asm("hlt");
}

void *sysi_alloc(size_t size)
{
    return pmm_alloc(size);
}

void sysi_free(void *address)
{
    pmm_free(address);
}