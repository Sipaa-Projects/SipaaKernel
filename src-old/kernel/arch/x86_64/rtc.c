// SKB_X86_64_ONLY

#include <arch/x86/io.h>

#define RTC_COMMAND         0x70
#define RTC_DATA            0x71
#define RTC_STATUS          0x0B

#define RTC_SECONDS         0x00
#define RTC_MINUTES         0x02
#define RTC_HOURS           0x04
#define RTC_DAY_OF_WEEK     0x06
#define RTC_DAY             0x07
#define RTC_MONTH           0x08
#define RTC_YEAR            0x09

char bcd;
unsigned char read_register(unsigned char reg) {
    __asm__ volatile("cli");
    outb(RTC_COMMAND, reg);
    return inb(RTC_DATA);
    __asm__ volatile("sti");
}

void write_register(unsigned char reg, unsigned char value) {
    __asm__ volatile("cli");
    outb(RTC_COMMAND, reg);
    outb(RTC_DATA, value);
    __asm__ volatile("sti");
}

unsigned char bcd2bin(unsigned char in_bcd) {
    return (bcd) ? ((in_bcd >> 4) * 10) + (in_bcd & 0x0F) : in_bcd;
}

/**void hal_get_time(hal_time *target) {
    target->seconds = bcd2bin(read_register(RTC_SECONDS));
    target->minutes = bcd2bin(read_register(RTC_MINUTES));
    target->hours = bcd2bin(read_register(RTC_HOURS));
    target->day_of_week = bcd2bin(read_register(RTC_DAY_OF_WEEK));
    target->day_of_month = bcd2bin(read_register(RTC_DAY));
    target->month = bcd2bin(read_register(RTC_MONTH));
    target->year = bcd2bin(read_register(RTC_YEAR));

    target->full[0] = target->seconds;
    target->full[1] = target->minutes;
    target->full[2] = target->hours;
    target->full[3] = target->day_of_month;
    target->full[4] = target->month;
    target->full[5] = target->year;
}**/

int rtc_init() {
    __asm__ volatile("cli");
    unsigned char status;
    status = read_register(RTC_STATUS);
    status |= 0x02;         // 24 hour clock
    status |= 0x10;         // update ended interrupts
    status &= ~0x20;        // no alarm interrupts
    status &= ~0x40;        // no periodic interrupt
    bcd = !(status & 0x04); // check if data type is BCD
    write_register(RTC_STATUS, status);
    __asm__ volatile("sti");
    return 0;
}