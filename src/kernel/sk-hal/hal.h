extern unsigned long kernel_stack[];

typedef struct {
    int seconds;
    int minutes;
    int hours;
    int day_of_week;
    int day_of_month;
    int month;
    int year;
    int full[6];
} hal_time;

int hal_init(void);

void hal_cpu_cli(void);
void hal_cpu_sti(void);
void hal_cpu_halt(void);
void halt_cpu_relax(void);
void hal_cpu_stop(void);
void hal_register_idt_handler(int num, void* handler, unsigned char ist);
void hal_get_time(hal_time *timeptr);