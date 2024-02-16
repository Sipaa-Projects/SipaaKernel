// SKB_NO_AARCH64 / SKB_NO_RISCV64

#include "ps2.h"
#include <logger/logger.h>
#include <arch/x86/io.h>

void init_ps2_mouse()
{
    uint8_t ack;

    outb(0xD4, 0x64);                    // tell the controller to address the mouse
    outb(0xF3, 0x60);                    // write the mouse command code to the controller's data port
    while(!(inb(0x64) & 1))
        asm("pause"); // wait until we can read
    ack = inb(0x60);                     // read back acknowledge. This should be 0xFA
    outb(0xD4, 0x64);                    // tell the controller to address the mouse
    outb(100, 0x60);                     // write the parameter to the controller's data port
    while(!(inb(0x64) & 1)) 
        asm("pause"); // wait until we can read
    ack = inb(0x60);                     // read back acknowledge. This should be 0xFA
}

void reset_ps2_device(unsigned char command) {
    // Wait until the input buffer is empty
    while (inb(PS2_COMMAND_PORT) & 0x02) {
        // Wait
    }

    // Send the reset command to the device
    outb(PS2_DATA_PORT, command);

    // Wait for a response from the device
    while (1) {
        if (!(inb(PS2_COMMAND_PORT) & 0x02) && (inb(PS2_DATA_PORT) == 0xFA)) {
            // Reset successful
            break;
        } else if (inb(PS2_COMMAND_PORT) & 0x01) {
            // Error occurred
            log(LT_ERROR, "ps2c", "Error resetting PS/2 device!");
            // Handle error or exit
            break;
        }
        // Wait
    }
}

void ps2_controller_init() {
    log(LT_INFO, "ps2c", "Initializing PS2 controller...\n");

    // Step 1: Disable Devices
    outb(PS2_COMMAND_PORT, PS2_COMMAND_DISABLE_FIRST_PORT);
    outb(PS2_COMMAND_PORT, PS2_COMMAND_DISABLE_SECOND_PORT);

    // Step 4: Flush The Output Buffer
    while (inb(PS2_STATUS_PORT) & PS2_STATUS_OUTPUT_BUFFER_FULL) {
        inb(PS2_DATA_PORT);
    }

    // Step 5: Set the Controller Configuration Byte
    outb(PS2_COMMAND_PORT, PS2_COMMAND_READ_CONFIG_BYTE);
    unsigned char configByte = inb(PS2_DATA_PORT);
    configByte &= ~(PS2_CONFIG_FIRST_PORT_INTERRUPT | PS2_CONFIG_SECOND_PORT_INTERRUPT |
                    PS2_CONFIG_FIRST_PORT_TRANSLATION);
    outb(PS2_COMMAND_PORT, PS2_COMMAND_WRITE_CONFIG_BYTE);
    outb(PS2_DATA_PORT, configByte);

    // Step 6: Perform Controller Self Test
    outb(PS2_COMMAND_PORT, PS2_COMMAND_CONTROLLER_SELF_TEST);
    if (inb(PS2_DATA_PORT) != 0x55) {
        log(LT_ERROR, "ps2c", "Self-test failed!");
        // Handle error or exit
    }

    // Step 7: Determine If There Are 2 Channels
    outb(PS2_COMMAND_PORT, PS2_COMMAND_ENABLE_SECOND_PORT);
    configByte = inb(PS2_COMMAND_PORT);
    if (configByte & PS2_CONFIG_SECOND_PORT_CLOCK) {
        // Dual channel supported, disable second port for now
        outb(PS2_COMMAND_PORT, PS2_COMMAND_DISABLE_SECOND_PORT);
    }

    // Step 8: Perform Interface Tests
    outb(PS2_COMMAND_PORT, PS2_COMMAND_TEST_SECOND_PORT);
    if (inb(PS2_DATA_PORT) != 0x00) {
        log(LT_ERROR, "ps2c", "Second PS/2 port test failed!");
        // Handle error or exit
    }

    // Step 9: Enable Devices
    outb(PS2_COMMAND_PORT, PS2_COMMAND_ENABLE_FIRST_PORT);
    if (configByte & PS2_CONFIG_SECOND_PORT_CLOCK) {
        outb(PS2_COMMAND_PORT, PS2_COMMAND_ENABLE_SECOND_PORT);
    }

    //init_ps2_mouse();

    // PS/2 Controller is now initialized
    log(LT_INFO, "ps2c", "Initialized successfully!\n");
}