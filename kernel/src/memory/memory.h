#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstddef>

namespace Sk {
namespace Memory {

class BasicMemoryManagement {

public:
    static void MemorySet(void* start, uint8_t value, size_t size);
    static void MemorySet16(void* start, uint16_t value, size_t size);
    static void MemorySet32(void* start, uint32_t value, size_t size);
    static void MemorySet64(void* start, uint64_t value, size_t size);
    static void MemoryCopy(void* Destination, void* Source, size_t Size);

};

} // namespace name    
} // namespace Sk


#endif