#pragma once

#include <arch/io.h>
#include <stdint.h>
#include <video/video.h>

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000100
#define PS2Rightbutton 0b00000010
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

namespace Sk {
namespace Dev {

struct PS2MouseStatus {
    int DeltaX;
    int DeltaY;
    int X;
    int Y;
};

class PS2 {
public:
    static uint8_t MouseCycle;
    static uint8_t MousePacket[4];
    static bool MousePacketReady;
    static PS2MouseStatus CurrentMouseStatus;
    static PS2MouseStatus OldMouseStatus;

    static void MouseWait();
    static void MouseWaitInput();
    static void MouseWrite(uint8_t value);
    static uint8_t MouseRead();
    static void MouseProcessPacket();
    static void MouseHandler(uint8_t data);
    static void Init();
};

}
}