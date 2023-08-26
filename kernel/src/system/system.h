#pragma once

#include <stdint.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

namespace Sk {

class System {
private:
    static void _panic(char *msg, char *file, char *line);
public:
    static void panic(char *msg)
    {
        _panic(msg, __FILE__, __LINE__);
    }
};

}