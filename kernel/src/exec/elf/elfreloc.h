#pragma once

#include "elf.h"

namespace Sk {
namespace Exec {

class Reloc{

public:
    static el_status Apply(el_ctx *ctx, Elf_Rel *rel);
    static el_status ApplyA(el_ctx *ctx, Elf_RelA *rela);

};

}
}
