#ifndef __SK_PRESTART_RET
#define __SK_PRESTART_RET

#define SK_RETURN_HLT 0 // Lock in a loop that halts SipaaKernel (interrupts enabled)
#define SK_RETURN_CLI_HLT 2 // Lock in a loop that halts SipaaKernel (interrupts disabled)
#define SK_RETURN_LOOP 1 // Lock in a loop, without disabling interrupts or halting

#endif