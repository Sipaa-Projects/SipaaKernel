// SKB_i686_ONLY

#include <stdint.h>

uint64_t __udivdi3(uint64_t numerator, uint64_t denominator) {
    if (denominator == 0) {
        // Handle division by zero, as needed.
        return 0; // Placeholder for the sake of the example.
    }

    uint64_t quotient;
    uint32_t high_numerator = numerator >> 32;
    uint32_t high_denominator = denominator >> 32;

    asm (
        "divl %[denominator];"
        : "=A"(quotient)   // Output: quotient in edx:eax
        : "A"(numerator),  // Input: numerator in edx:eax
          [denominator] "rm"(denominator) // Input: denominator
        : "cc"             // Clobbered: condition codes
    );

    return quotient;
}
