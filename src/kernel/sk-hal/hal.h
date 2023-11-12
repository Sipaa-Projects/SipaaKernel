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

/// @brief Initialize the HAL
int hal_init(void);

/// @brief Disable interrupts
void hal_cpu_cli(void);

/// @brief Enable interrupts
void hal_cpu_sti(void);

/// @brief Halt CPU
void hal_cpu_halt(void);

/// @brief Relax/Pause CPU
void halt_cpu_relax(void);

/// @brief Disable interrupts then halt CPU
void hal_cpu_stop(void);

/// @brief Register an IDT handler (x86_64-specific)
void hal_register_idt_handler(int num, void* handler, unsigned char ist);

/// @brief Get the current time
/// @param timeptr A pointer to a memory space of the size of the 'hal_time' struct.
void hal_get_time(hal_time *timeptr);