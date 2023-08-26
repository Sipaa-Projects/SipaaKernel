#include "ps2.h"

#include <global.h>

using namespace Sk::Arch;

namespace Sk {
namespace Dev {

uint8_t PS2::MouseCycle = 0;
uint8_t PS2::MousePacket[4];
bool PS2::MousePacketReady = false;
PS2MouseStatus PS2::CurrentMouseStatus;
PS2MouseStatus PS2::OldMouseStatus;

void PS2::MouseWait()
{
    #ifndef __aarch64__
    uint64_t timeout = 100000;
    while (timeout--){
        if ((Io::InB(0x64) & 0b10) == 0){
            return;
        }
    }
    #endif
}

void PS2::MouseWaitInput()
{
    #ifndef __aarch64__
    uint64_t timeout = 100000;
    while (timeout--){
        if (Io::InB(0x64) & 0b1){
            return;
        }
    }
    #endif
}

void PS2::MouseWrite(uint8_t value)
{
    #ifndef __aarch64__
    MouseWait();
    Io::OutB(0x64, 0xD4);
    MouseWait();
    Io::OutB(0x60, value);
    #endif
}

uint8_t PS2::MouseRead()
{
    #ifndef __aarch64__
    MouseWaitInput();
    return Io::InB(0x60);
    #else
    return 0;
    #endif
}

void PS2::MouseHandler(uint8_t data)
{
    switch(MouseCycle)
    {
        case 0:
            if (MousePacketReady) break;
            if (data & 0b00001000 == 0) break;
            MousePacket[0] = data;
            MouseCycle++;
            break;
        case 1:
            if (MousePacketReady) break;
            MousePacket[1] = data;
            MouseCycle++;
            break;
        case 2:
            if (MousePacketReady) break;
            MousePacket[2] = data;
            MousePacketReady = true;
            MouseCycle = 0;
            break;
    }
}

void PS2::MouseProcessPacket(){
    bool xNegative, yNegative, xOverflow, yOverflow;
    
    if (!MousePacketReady) 
        return;

    if (MousePacket[0] & PS2XSign)
        xNegative = true;
    else 
        xNegative = false;

    if (MousePacket[0] & PS2YSign)
        yNegative = true;
    else 
        yNegative = false;

    if (MousePacket[0] & PS2XOverflow)
        xOverflow = true;
    else 
        xOverflow = false;

    if (MousePacket[0] & PS2YOverflow)
        yOverflow = true;
    else 
        yOverflow = false;

    if (!xNegative)
    {
        CurrentMouseStatus.X += MousePacket[1];
        if (xOverflow){
            CurrentMouseStatus.X += 255;
        }
    } 
    else
    {
        MousePacket[1] = 256 - MousePacket[1];
        CurrentMouseStatus.X -= MousePacket[1];
        if (xOverflow){
            CurrentMouseStatus.X -= 255;
        }
    }

    if (!yNegative)
    {
        CurrentMouseStatus.Y -= MousePacket[2];
        if (yOverflow){
            CurrentMouseStatus.Y -= 255;
        }
    } 
    else
    {
        MousePacket[2] = 256 - MousePacket[2];
        CurrentMouseStatus.Y += MousePacket[2];
        if (yOverflow){
            CurrentMouseStatus.Y += 255;
        }
    }

    if (CurrentMouseStatus.X < 0) CurrentMouseStatus.X = 0;
    if (CurrentMouseStatus.X > Global::Framebuffer.Width - 1) CurrentMouseStatus.X = Global::Framebuffer.Width - 1;
        
    if (CurrentMouseStatus.Y < 0) CurrentMouseStatus.Y = 0;
    if (CurrentMouseStatus.Y > Global::Framebuffer.Height - 1) CurrentMouseStatus.Y = Global::Framebuffer.Height - 1;
        
        /**GlobalRenderer->ClearMouseCursor(MousePointer, MousePositionOld);
        GlobalRenderer->DrawOverlayMouseCursor(MousePointer, MousePosition, 0xffffffff);

        if (MousePacket[0] & PS2Leftbutton){
            GlobalRenderer->PutChar('a', MousePosition.X, MousePosition.Y);
        }
        if (MousePacket[0] & PS2Middlebutton){
            
        }
        if (MousePacket[0] & PS2Rightbutton){
            uint32_t colour = GlobalRenderer->Colour;
            GlobalRenderer->Colour = 0x0000ff00;
            GlobalRenderer->PutChar('a', MousePosition.X, MousePosition.Y);
            GlobalRenderer->Colour = colour;
        }**/

    MousePacketReady = false;
    OldMouseStatus = CurrentMouseStatus;
}

void PS2::Init()
{
    #ifndef __aarch64__
    Io::OutB(0x64, 0xA8); //enabling the auxiliary device - mouse

    MouseWait();
    Io::OutB(0x64, 0x20); //tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
    uint8_t status = Io::InB(0x60);
    status |= 0b10;
    MouseWait();
    Io::OutB(0x64, 0x60);
    MouseWait();
    Io::OutB(0x60, status); // setting the correct bit is the "compaq" status byte

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
    #endif
}

}
}