//
// Created by os on 5/8/24.
//

#include "../h/riscv.hpp"
#include "../h/error_codes.hpp"
#include "../lib/console.h"
#include "../h/memoryAllocator.hpp"
#include "../h/tcb.hpp"
#include "../h/sem_minor.hpp"
#include "../h/syscall_c.hpp"


const uint64 USER_CAUSE_ERROR = 0x0000000000000008UL;
const uint64 SYSTEM_CAUSE_ERROR = 0x0000000000000009UL;

using Body = void (*)(void*);


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
        uint64 volatile sstatus = r_sstatus();

        uint64 error_code = Riscv::r_a0();
        switch (error_code) {
            case(MEM_ALLOC):

                size_t size;
                __asm__ volatile ("mv %0, a1" : "=r" (size));
                MemoryAllocator::mem_alloc(size);

                __asm__ volatile("sd a0, 80(s0)");

                break;

            case(MEM_FREE):

                void* segment;
                __asm__ volatile("mv %0, a1" : "=r" (segment));
                MemoryAllocator::mem_free(segment);

                __asm__ volatile("sd a0, 80(s0)");

                break;
            case(THREAD_CREATE):
                TCB** test;
                Body body;
                void* arg;
                __asm__ volatile("mv %0, a1" : "=r" (test));
                __asm__ volatile("mv %0, a2" : "=r" (body));
                __asm__ volatile("mv %0, a7" : "=r" (arg));

                *test =  TCB::createThread(body,arg);

                if(*test != nullptr) { __asm__ volatile("li a0, 0"); }
                else { __asm__ volatile("li a0, -1"); }

                __asm__ volatile("sd a0, 80(s0)");

                break;
            case(THREAD_EXIT):
                TCB::running->setFinished(true);
                TCB::dispatch();
                __asm__ volatile("li t0, 0"); // Should not enter here, right?
                __asm__ volatile("sd t0, 80(s0)");
                break;
            case(THREAD_DISPATCH):
                TCB::dispatch();
                break;

            case(SEM_OPEN):
                Sem_minor** handle_opening;
                unsigned init;

                __asm__ volatile("mv %0, a1" : "=r" (handle_opening));
                __asm__ volatile("mv %0, a2" : "=r" (init));


                if(Sem_minor::createSem_minor(handle_opening,init)){
                    __asm__ volatile("li a0, 0");
                }
                else {
                    __asm__ volatile("li a0, -1");
                }

                __asm__ volatile("sd a0, 80(s0)");
                break;

            case(SEM_CLOSE):
                Sem_minor* handle_closing;
                __asm__ volatile ("mv %0, a1" : "=r" (handle_closing));
                handle_closing->close();
                __asm__ volatile("sd a0, 80(s0)");
                break;

            case(SEM_WAIT):
                Sem_minor* handle_to_wait;
                __asm__ volatile ("mv %0, a1" : "=r" (handle_to_wait));
                handle_to_wait->wait();
                __asm__ volatile("sd a0, 80(s0)");
                break;

            case(SEM_SIGNAL):
                Sem_minor* handle_to_signal;
                __asm__ volatile ("mv %0, a1" : "=r" (handle_to_signal));
                handle_to_signal->signal();
                __asm__ volatile("sd a0, 80(s0)");
                break;

            case(SEM_TRYWAIT):
                Sem_minor* handle_to_trywait;
                __asm__ volatile ("mv %0, a1" : "=r" (handle_to_trywait));
                handle_to_trywait->trywait();
                __asm__ volatile("sd a0, 80(s0)");
                break;

            default:
                break;
        }
        w_sstatus(sstatus);
        w_sepc(new_sepc);

    }
    else if(scause==0x8000000000000001UL)
    {
        mc_sip(SIP_SSIE);
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        //__putc('T');
        //__putc('\n');
        //TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);

    }
    else if (scause == 0x8000000000000009UL)
    {
//          interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
    }

    //w_sepc(new_sepc);

}