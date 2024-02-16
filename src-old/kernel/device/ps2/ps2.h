#pragma once

#include <stdint.h>

// PS/2 Controller IO Ports
#define PS2_DATA_PORT (uint16_t)0x60
#define PS2_STATUS_PORT (uint16_t)0x64
#define PS2_COMMAND_PORT (uint16_t)0x64

// PS/2 Controller Commands
#define PS2_COMMAND_DISABLE_FIRST_PORT (uint8_t)0xAD
#define PS2_COMMAND_DISABLE_SECOND_PORT (uint8_t)0xA7
#define PS2_COMMAND_READ_CONFIG_BYTE (uint8_t)0x20
#define PS2_COMMAND_WRITE_CONFIG_BYTE (uint8_t)0x60
#define PS2_COMMAND_CONTROLLER_SELF_TEST (uint8_t)0xAA
#define PS2_COMMAND_ENABLE_SECOND_PORT (uint8_t)0xA8
#define PS2_COMMAND_TEST_SECOND_PORT (uint8_t)0xA9
#define PS2_COMMAND_ENABLE_FIRST_PORT (uint8_t)0xAE

// PS/2 Controller Status Register Bits
#define PS2_STATUS_OUTPUT_BUFFER_FULL (uint8_t)0x01
#define PS2_STATUS_INPUT_BUFFER_FULL (uint8_t)0x02

// PS/2 Controller Configuration Byte Bits
#define PS2_CONFIG_FIRST_PORT_INTERRUPT (uint8_t)0x01
#define PS2_CONFIG_SECOND_PORT_INTERRUPT (uint8_t)0x02
#define PS2_CONFIG_SYSTEM_FLAG (uint8_t)0x04
#define PS2_CONFIG_FIRST_PORT_CLOCK (uint8_t)0x10
#define PS2_CONFIG_SECOND_PORT_CLOCK (uint8_t)0x20
#define PS2_CONFIG_FIRST_PORT_TRANSLATION (uint8_t)0x40

void ps2_controller_init();
void init_ps2_mouse();