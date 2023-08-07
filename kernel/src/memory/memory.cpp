#include "memory.h"
#include <stddef.h>

namespace Sk {
namespace Memory {

void BasicMemoryManagement::MemorySet(void* start, uint8_t value, size_t size) {
    for (uint64_t i = 0; i < size; i += sizeof(uint8_t)){
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}       

void BasicMemoryManagement::MemorySet16(void* start, uint16_t value, size_t size) {
    for (uint64_t i = 0; i < size; i += sizeof(uint16_t)){
        *(uint16_t*)((uint64_t)start + i) = value;
    }
}

void BasicMemoryManagement::MemorySet32(void* start, uint32_t value, size_t size) {
    for (uint64_t i = 0; i < size; i += sizeof(uint32_t)){
        *(uint32_t*)((uint64_t)start + i) = value;
    }
}

void BasicMemoryManagement::MemorySet64(void* start, uint64_t value, size_t size) {
    for (uint64_t i = 0; i < size; i += sizeof(uint64_t)){
        *(uint64_t*)((uint64_t)start + i) = value;
    }
}

void BasicMemoryManagement::MemoryCopy(void* Destination, void* Source, size_t Size)
{
    long d0, d1, d2; 
    asm volatile(
            "rep ; movsq\n\t movq %4,%%rcx\n\t""rep ; movsb\n\t": "=&c" (d0),
            "=&D" (d1),
            "=&S" (d2): "0" (Size >> 3), 
            "g" (Size & 7), 
            "1" (Destination),
            "2" (Source): "memory"
    );  
}

}
}