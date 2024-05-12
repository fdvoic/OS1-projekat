//
// Created by os on 5/8/24.
//

#include "../h/riscv.hpp"
#include "../h/error_codes.hpp"
#include "../lib/console.h"
#include "../h/memoryAllocator.hpp"

const uint64 USER_CAUSE_ERROR = 0x0000000000000008UL;
const uint64 SYSTEM_CAUSE_ERROR = 0x0000000000000009UL;


void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap() {

    uint64 scause = r_scause();
    if(scause == USER_CAUSE_ERROR || scause == SYSTEM_CAUSE_ERROR)
    {
        // Interrupt: No;
        // Cause code: Environment call from U-mode or S-mode;
        uint64 volatile new_sepc = r_sepc() + 4;

        uint64 error_code=Riscv::r_a0();

        switch (error_code) {
            case(MEM_ALLOC):
                //uint64 p=0;

                size_t size;
                __asm__ volatile ("mv %0, a1" : "=r" (size));

                MemoryAllocator::mem_alloc(size);
                __asm__ volatile("sd a0, 80(s0)");

                __putc('u');
                __putc('s');
                __putc('p');
                __putc('e');
                __putc('o');
                __putc('\n');

                break;

            case(MEM_FREE):
                void* segment;
                __asm__ volatile("mv %0, a1" : "=r" (segment));

                MemoryAllocator::mem_free(segment);
                __asm__ volatile("sd a0, 80(s0)");

                __putc('U');
                __putc('s');
                __putc('P');
                __putc('e');
                __putc('o');
                __putc('\n');

                break;

            default:
                break;
        }
        w_sepc(new_sepc);
    }


}