// Sample program to test SK's ELF loader.
typedef enum HelloWorldType
{
    HELLO,
    GOODBYE,
    FOOBOO
} HelloWorldTypeT;

typedef struct HelloWorld
{
    HelloWorldTypeT type;
    char *hello;
} HelloWorldT;

char *fooboo = "FOOBOO";

unsigned long long int sipaakernel_now_detect_symbols_babe()
{
    return 0xCAFEBABE;
}

void foo(HelloWorldT *hwt)
{
    (void)hwt;
}

// In fact, GCC does check for the _start function instead of _main. It is defined in the cr0.s file of the LibC you are using.
int _start()
{
    HelloWorldT hwt = { };
    hwt.type = FOOBOO;
    hwt.hello = fooboo;

    foo(&hwt);
    sipaakernel_now_detect_symbols_babe();
    return 0;
}