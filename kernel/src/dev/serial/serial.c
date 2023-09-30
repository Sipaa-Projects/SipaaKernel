#include <dev/serial/serial.h>

#include <memory/memory.h>
#include <lib/io.h>
#include <lib/log.h>

#define SERIAL_PORT 0x3f8

bool serial_isready()
{
    return inb(SERIAL_PORT + 5) & 0x20;
}

void serial_write(dev *self, const char *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        while (!serial_isready())
            ;;
        
        outb(SERIAL_PORT, buf[i]);
    }
}
void serial_read(dev *self, char *buf, size_t len) {
    // Reading from serial console can't be done.
    buf = NULL;
    return;
}

void *serial_init()
{
    outb(SERIAL_PORT + 1, 0x00);
    outb(SERIAL_PORT + 3, 0x80);
    outb(SERIAL_PORT + 0, 0x01);
    outb(SERIAL_PORT + 1, 0x00);
    outb(SERIAL_PORT + 3, 0x03);
    outb(SERIAL_PORT + 2, 0xC7);
    outb(SERIAL_PORT + 4, 0x0B);

    void *d = malloc(sizeof(dev));

    if (!d)
    {
        log(LOGTYPE_WARNING, "serial_init: malloc returned NULL. You may encounter problems later.");
    }

    dev *d2 = (dev *)d;
    d2->write = &serial_write;
    d2->read = &serial_read;

    return d;
}