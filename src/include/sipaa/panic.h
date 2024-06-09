/*
 * @file src/include/sipaa/panic.h
 * @brief Handles serious & fatal errors.
 * @copyright Copyright (C) 2024-present Sipaa Projects
 */

#pragma once

/// @brief May be called when a fatal error occurs.
/// @param fmt The message format
void Sk_Panic(const char *fmt, ...);