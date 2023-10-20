#ifndef __DEVICE_H__
#define __DEVICE_H__

typedef struct dev {
    void (*write)(struct Device *self, const char *buf, int len);
    void (*read)(struct Device *self, char *buf, int len);
} dev;

#endif