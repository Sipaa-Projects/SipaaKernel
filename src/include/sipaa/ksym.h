#pragma once

#include <sipaa/exec/elf.h>

/// @brief Initialize the kernel symbol table
void KernelSymbols_Initialize();

/// @brief Get the symbol name from an offset in the string table.
/// @param offset The offset in the string table. Usually located in <Elf64_Sym>.st_name
/// @return A null-terminated string.
char *KernelSymbols_GetSymbolName(uint32_t offset);

/// @brief Get a ELF symbol header from an instruction pointer
/// @param ip The instruction pointer
/// @returns An ELF symbol.
Elf64_Sym *KernelSymbols_GetFromIP(uint64_t ip);