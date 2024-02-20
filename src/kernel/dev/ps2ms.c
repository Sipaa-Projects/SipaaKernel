#include <sipaa/conio.h>
#include <sipaa/i686/ps2.h>
#include <sipaa/i686/isr.h>
#include <sipaa/i686/io.h>
#include <sipaa/logger.h>

// Mouse

#define MOUSE_CMD_RESET 0xFF
#define MOUSE_CMD_RESEND 0xFE
#define MOUSE_CMD_SET_DEFAULTS 0xF6
#define MOUSE_CMD_DISABLE_PACKET_STREAMING 0xF5
#define MOUSE_CMD_ENABLE_PACKET_STREAMING 0xF4
#define MOUSE_CMD_SAMPLE_RATE 0xF3
#define MOUSE_CMD_MOUSE_ID 0xF2
#define MOUSE_CMD_REQUEST_SINGLE_PACKET 0xEB
#define MOUSE_CMD_STATUS 0xE9
#define MOUSE_CMD_RESOLUTION 0xE8

#define MOUSE_ACKNOWLEDGE 0xFA

#define PS2_CMD_PORT 0x64    // read/write PS/2 command register
#define MOUSE_DATA_PORT 0x60 // PS/2 data port register see https://wiki.osdev.org/%228042%22_PS/2_Controller

typedef struct
{
    uint8_t left_button : 1;
    uint8_t right_button : 1;
    uint8_t middle_button : 1;
    uint8_t always_1 : 1;
    uint8_t x_sign : 1;
    uint8_t y_sign : 1;
    uint8_t x_overflow : 1;
    uint8_t y_overflow : 1;
} MOUSE_STATUS;

int g_mouse_x_pos = 0, g_mouse_y_pos = 0;
MOUSE_STATUS g_status;
int screenwidth = 0;
int screenheight = 0;
int mstatus = 0;
char sbyte;

int ps2ms_getx() {
    return g_mouse_x_pos;
}

int ps2ms_gety() {
    return g_mouse_y_pos;
}

char ps2ms_getstatus() {
    return sbyte;
}

void mouse_wait(int type) {
    uint32_t time_out = 100000;
    if (type == 0) {
        // suspend until status is 1
        while (time_out--) {
            if ((inb(PS2_CMD_PORT) & 1) == 1) {
                return;
            }
        }
        return;
    } else {
        while (time_out--) {
            if ((inb(PS2_CMD_PORT) & 2) == 0) {
                return;
            }
        }
    }
}

void mouse_write(uint8_t data) {
    // sending write command
    mouse_wait(1);
    outb(PS2_CMD_PORT, 0xD4);
    mouse_wait(1);
    // finally write data to port
    outb(MOUSE_DATA_PORT, data);
}

uint8_t mouse_read() {
    mouse_wait(0);
    return inb(MOUSE_DATA_PORT);
}

void get_mouse_status(char status_byte, MOUSE_STATUS *status) {
    mstatus = sizeof(status);
    sbyte = status_byte;
    if (status_byte & 0x01)
        status->left_button = 1;
    if (status_byte & 0x02)
        status->right_button = 1;
    if (status_byte & 0x04)
        status->middle_button = 1;
    if (status_byte & 0x08)
        status->always_1 = 1;
    if (status_byte & 0x10)
        status->x_sign = 1;
    if (status_byte & 0x20)
        status->y_sign = 1;
    if (status_byte & 0x40)
        status->x_overflow = 1;
    if (status_byte & 0x80)
        status->y_overflow = 1;
}

void ps2_mouse_handler(REGISTERS *r)
{
    conio_println("Mouse handler!");
    static uint8_t mouse_cycle = 0;
    static char mouse_byte[3];

    switch (mouse_cycle) {
        case 0:
            mouse_byte[0] = mouse_read();
            get_mouse_status(mouse_byte[0], &g_status);
            mouse_cycle++;
            break;
        case 1:
            mouse_byte[1] = mouse_read();
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[2] = mouse_read();
            g_mouse_x_pos = g_mouse_x_pos + mouse_byte[1];
            g_mouse_y_pos = g_mouse_y_pos - mouse_byte[2];

            if (g_mouse_x_pos < 0)
                g_mouse_x_pos = 0;
            if (g_mouse_y_pos < 0)
                g_mouse_y_pos = 0;
            if (g_mouse_x_pos > screenwidth)
                g_mouse_x_pos = screenwidth - 1;
            if (g_mouse_y_pos > screenheight)
                g_mouse_y_pos = screenheight - 1;

            mouse_cycle = 0;
            break;
    }
    isr_end_interrupt(IRQ_BASE + 12);
}

/**
 * available rates 10, 20, 40, 60, 80, 100, 200
 */
void set_mouse_rate(uint8_t rate) {
    uint8_t status;

    outb(MOUSE_DATA_PORT, MOUSE_CMD_SAMPLE_RATE);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        //printf("error: failed to send mouse sample rate command\n");
        return;
    }
    outb(MOUSE_DATA_PORT, rate);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        //printf("error: failed to send mouse sample rate data\n");
        return;
    }
}

extern void ps2_keyboard_handler(REGISTERS *r);

void ps2ms_init()
{
    log(LT_INFO, "PS2/Mouse", "Initializing...\n");
    // Init mouse
    uint8_t status;

    g_mouse_x_pos = 5;
    g_mouse_y_pos = 2;
    
    screenwidth = 800;
    screenheight = 600;

    ////printf("initializing mouse...\n");

    // enable mouse device
    log(LT_INFO, "PS2/Mouse", "Enabling mouse...\n");
    mouse_wait(1);
    outb(PS2_CMD_PORT, 0xA8);
    log(LT_INFO, "PS2/Mouse", "Enabled\n");

    // print mouse id
    log(LT_INFO, "PS2/Mouse", "Setting mouse rate to 10...\n");
    set_mouse_rate(10);

    //outb(MOUSE_DATA_PORT, MOUSE_CMD_RESOLUTION);
    //outb(MOUSE_DATA_PORT, 0);

    // enable the interrupt
    log(LT_INFO, "PS2/Mouse", "Enabling interrupt...\n");
    mouse_wait(1);
    outb(PS2_CMD_PORT, 0x20);
    mouse_wait(0);
    // get and set second bit
    status = (inb(MOUSE_DATA_PORT) | 2);
    // write status to port
    mouse_wait(1);
    outb(PS2_CMD_PORT, MOUSE_DATA_PORT);
    mouse_wait(1);
    outb(MOUSE_DATA_PORT, status);

    // set mouse to use default settings
    log(LT_INFO, "PS2/Mouse", "Make the mouse use default settings...\n");
    mouse_write(MOUSE_CMD_SET_DEFAULTS);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        log(LT_ERROR, "PS2/Mouse", "Error while initializing mouse: failed to set default mouse settings\n");
        return;
    }

    // enable packet streaming to receive
    log(LT_INFO, "PS2/Mouse", "Enabling packet streaming...\n");
    mouse_write(MOUSE_CMD_ENABLE_PACKET_STREAMING);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        log(LT_ERROR, "PS2/Mouse", "Error while initializing mouse to enable mouse packet streaming\n");
        return;
    }

    // Register mouse handler
    isr_register_interrupt_handler(IRQ_BASE + 12, ps2_mouse_handler);
    log(LT_INFO, "PS2/Mouse", "Initialized\n");
}