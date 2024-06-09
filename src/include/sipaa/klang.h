/*
 * @file src/include/sipaa/klang.h
 * @brief Provides some translations for the user to understand the kernel output in his native language.
 */

#pragma once

#ifdef __SIPAAKERNEL_KLANG_EN

#include <sipaa/klang/klang-en.h>

#elif defined(__SIPAAKERNEL_KLANG_FR)

#include <sipaa/klang/klang-fr.h>

#elif defined(__SIPAAKERNEL_KLANG_IT)

#include <sipaa/klang/klang-it.h>

#else

// Use english.
#include <sipaa/klang/klang-en.h>

#endif