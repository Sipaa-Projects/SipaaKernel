#pragma once

#include <cstdint>

namespace SipaaKernel
{
    class SingleFramebuffer
    {
    private:
        /* data */
    public:
        uint64_t Width, Height, BPP;
        void *Address;
        
        SingleFramebuffer(/* args */);
        ~SingleFramebuffer();
    };
    
    SingleFramebuffer::SingleFramebuffer(/* args */)
    {
    }
    
    SingleFramebuffer::~SingleFramebuffer()
    {
    }
    
} // namespace SipaaKernel
