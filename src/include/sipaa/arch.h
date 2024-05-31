/*
 * @file src/include/sipaa/arch.h
 * @brief Basic architecture-dependant options.
 * @copyright Copyright (C) 2024-present Sipaa Projects
 */

#pragma once

#include <stdint.h>

/// @brief Dump the registers of the CPU.
/// @return A structure containing all the registers
void *Arch_DumpRegisters();

/// @brief Initialize the 
/// @return A structure containing all the registers
void *Arch_InitializeS1();