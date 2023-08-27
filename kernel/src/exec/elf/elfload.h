/* Copyright © 2014, Owen Shepherd
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "elfarch.h"
#include "elf.h"

#ifdef DEBUG
#include <console/console.h>
#define EL_DEBUG(...) Sk::Logging::Logger::Log(Sk::Logging::LogType::LogType_Debug, __VA_ARGS__)
#else
#define EL_DEBUG(...) do {} while(0)
#endif

namespace Sk {
namespace Exec {

typedef enum {
    EL_OK         = 0,

    EL_EIO,
    EL_ENOMEM,

    EL_NOTELF,
    EL_WRONGBITS,
    EL_WRONGENDIAN,
    EL_WRONGARCH,
    EL_WRONGOS,
    EL_NOTEXEC,
    EL_NODYN,
    EL_BADREL,

} el_status;

typedef struct el_ctx {
    bool (*pread)(struct el_ctx *ctx, void *dest, size_t nb, size_t offset);

    /* base_load_* -> address we are actually going to load at
     */
    Elf_Addr
        base_load_paddr,
        base_load_vaddr;

    /* size in memory of binary */
    Elf_Addr memsz;

    /* required alignment */
    Elf_Addr align;

    /* ELF header */
    Elf_Ehdr  ehdr;

    /* Offset of dynamic table (0 if not ET_DYN) */
    Elf_Off  dynoff;
    /* Size of dynamic table (0 if not ET_DYN) */
    Elf_Addr dynsize;
} el_ctx;


typedef void* (*el_alloc_cb)(
    el_ctx *ctx,
    Elf_Addr phys,
    Elf_Addr virt,
    Elf_Addr size);

typedef struct {
    Elf_Off  tableoff;
    Elf_Addr tablesize;
    Elf_Addr entrysize;
} el_relocinfo;

class ElfLoader {
public:
    /* find all information regarding relocations of a specific type.
    *
    * pass DT_REL or DT_RELA for type
    * sets ri->entrysize = 0 if not found
    */
    static el_status FindRelocs(el_ctx *ctx, el_relocinfo *ri, uint32_t type);

    static el_status Load(el_ctx *ctx, el_alloc_cb alloccb);
    static el_status PRead(el_ctx *ctx, void *def, size_t nb, size_t offset);
    static el_status Init(el_ctx *ctx);

    /* find a dynamic table entry
    * returns the entry on success, dyn->d_tag = DT_NULL on failure
    */
    static el_status FindDyn(el_ctx *ctx, Elf_Dyn *dyn, uint32_t type);

    /* Relocate the loaded executable */
    static el_status Relocate(el_ctx *ctx);

    /* find the next phdr of type \p type, starting at \p *i.
    * On success, returns EL_OK with *i set to the phdr number, and the phdr loaded
    * in *phdr.
    *
    * If the end of the phdrs table was reached, *i is set to -1 and the contents
    * of *phdr are undefined
    */
    static el_status FindPHDR(el_ctx *ctx, Elf_Phdr *phdr, uint32_t type, unsigned *i);
};

}
}