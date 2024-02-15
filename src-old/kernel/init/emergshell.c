#include <device/conio/conio.h>
#include <device/serial/serial.h>
#include <logger/logger.h> // TODO: remove this as it's only used when the user presses 'enter', and replace it with a command parser

bool is_up_arrow(char c) {
    return c == '\e' && serial_readc() == '[' && serial_readc() == 'A';
}

bool is_down_arrow(char c) {
    return c == '\e' && serial_readc() == '[' && serial_readc() == 'B';
}

bool is_left_arrow(char c) {
    return c == '\e' && serial_readc() == '[' && serial_readc() == 'D';
}

bool is_right_arrow(char c) {
    return c == '\e' && serial_readc() == '[' && serial_readc() == 'C';
}

int emergshell_main()
{
    conio_clear();

    serial_puts("SipaaKernel Emergency Shell\n");
    conio_write("SipaaKernel Emergency Shell\n", 1, 28);
    serial_puts("Use the serial console to write commands\n\n");
    conio_write("Use the serial console to write commands\n\n", 1, 42);

    while (1) {
        serial_puts("I have no name@skpc $> ");
        conio_write("I have no name@skpc> ", 1, 21);

        int cx = 0;

        while (1) {
            char c = serial_readc();
            char buf[512];
            
            if (c == '\r') {
                serial_putc('\r');
                serial_putc('\n');

                conio_write("\n", 1, 1);
                
                log(LT_INFO, "sh", "Buffer: \"%s\"\n", buf);
                break;
            } else if (is_left_arrow(c)) {
                if (cx > 0) {
                    serial_puts("\b");
                    conio_write("\b", 1, 1);
                    cx--;
                }
            } else if (is_right_arrow(c)) {
                if (cx < sizeof(buf) - 1 && buf[cx] != '\0') {
                    // Move existing characters to the right
                    for (int i = cx + 1; i > 0; i--) {
                        buf[i] = buf[i - 1];
                    }

                    // Insert the character
                    buf[cx] = serial_readc();
                    
                    // Update the screen
                    conio_write(&buf[cx], 1, 1);

                    // Move the cursor to the right
                    for (int i = cx; i > 0; i--) {
                        serial_puts("\b");
                    }
                    cx++;
                }
            } else if (c == 127) {
                if (cx > 0) {
                    serial_puts("\b \b");
                    conio_write("\b \b", 1, 3);
                    buf[cx] = ' ';
                    cx--;
                }
            } else if (is_up_arrow(c)) {
                // Handle up arrow logic (e.g., command history)
                // Add your implementation here
            } else if (is_down_arrow(c)) {
                // Handle down arrow logic (e.g., command history)
                // Add your implementation here
            } else {
                serial_putc(c);
                conio_write(&c, 1, 1);
                buf[cx] = c;
                cx++;
            }
        }
    }

    return 0;
}