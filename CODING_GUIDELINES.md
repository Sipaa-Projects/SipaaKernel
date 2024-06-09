# SipaaKernel C Coding Guidelines

This document provides guidelines to make reading and understanding the SipaaKernel codebase easier and more enjoyable.

## Naming Convention

SipaaKernel uses a specific naming convention for different elements of the code. Here's a summary of the naming conventions used:

- **Enums and Structures**: PascalCase is used for the naming of enums and structures. All the variables & function also uses PascalCase. For typedefs, PascalCase is used with an additional uppercase 'T' at the end.
- **Functions and Variables**: A mix of PascalCase and snake_case is used for function and variable declarations.
- **Enum Values and Preprocessor Definitions**: Uppercase snake case is used for enum values.

Here's a breakdown of the naming convention for enums and structs:

- `<Component>`: The name of the component that the enum or struct is associated with. This should be written in PascalCase.
- `<Name>`: A descriptive, short name for the enum or struct. This should also be written in PascalCase.

Here are some examples to illustrate these guidelines:

```C
// Preprocessor definition using uppercase SNAKE_CASE for it's naming.
#define PMM_PAGE_SIZE 4096 

// Enum using PascalCase for its naming, and uppercase SNAKE_CASE for its contents
enum PmmStatus {
  PMM_SUCCESS = 0,
  PMM_ERROR
};

// Struct using PascalCase for its naming and its content variables naming
struct PmmPage {
  void* Address;
  bool Present;
};

// Typedef using PascalCase with an additional uppercase 'T' at the end
typedef struct PmmPage PmmPageT;

// Function & variable declaration using a mix of PascalCase & snake_case for its naming, and camelCase for its parameters naming
PmmPageT** Vmm_FreePages;
void Pmm_Initialize(void* memoryMap);
```

## Commenting Guidelines
We require that you put short & descriptive comments in your functions, and that you comment with `/// @` character followed by `brief`, `param` and `return` like that (VSCode automatically does it) (By the way, this is a real kernel function):

```c
/// @brief A pointer to the memory map
void* Pmm_MemoryMapAddress = NULL;

/// @brief Initialize the physical memory manager
/// @param memoryMapAddress A pointer to the memory map
void Pmm_Initialize(void* memoryMapAddress) {
    // Trigger a breakpoint interrupt for debugging
    __asm__ volatile("int $0x03");
}
```

## Endings
You may see a wonderful example of this guidelines applied into these files:
* src/include/sipaa/fs/vfs.h
* src/fs/vfs.c