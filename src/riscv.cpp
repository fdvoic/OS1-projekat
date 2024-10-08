//
// Created by os on 5/8/24.
//

#include "../h/riscv.hpp"
#include "../h/error_codes.hpp"
#include "../h/sem_minor.hpp"
#include "../h/IOConsole.hpp"

priorityQueueMorpheus Riscv::PQS;

const uint64 USER_CAUSE_ERROR = 0x0000000000000008UL;
const uint64 SYSTEM_CAUSE_ERROR = 0x0000000000000009UL;

using Body = void (*)(void*);

void Riscv::popSppSpie()
{
    if (TCB::running->getMode()) {
        ms_sstatus(SSTATUS_SPP);
    }
    else {
        mc_sstatus(SSTATUS_SPP);
    }
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
                void* stack;
                __asm__ volatile("mv %0, a1" : "=r" (test));
                __asm__ volatile("mv %0, a2" : "=r" (body));
                __asm__ volatile("mv %0, a6" : "=r" (arg));
                __asm__ volatile("mv %0, a7" : "=r" (stack));

                *test =  TCB::createThread(body,arg,stack);

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
                TCB::timeSliceCounter = 0;
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

            case(SEM_TIMEDWAIT):
                Sem_minor* handle_to_timed_wait;
                time_t sl;

                __asm__ volatile ("mv %0, a1" : "=r" (handle_to_timed_wait));
                __asm__ volatile ("mv %0, a2" : "=r" (sl));

                handle_to_timed_wait->timedwait(sl);

                __asm__ volatile("sd a0, 80(s0)");
                break;

            case(SEM_TRYWAIT):
                Sem_minor* handle_to_trywait;
                __asm__ volatile ("mv %0, a1" : "=r" (handle_to_trywait));
                handle_to_trywait->trywait();
                __asm__ volatile("sd a0, 80(s0)");
                break;

            case(TIME_SLEEP):
                time_t tim;
                __asm__ volatile ("mv %0, a1" : "=r" (tim));

                if(tim > 0)
                {
                    TCB::running->setAsleep(true);
                    PQS.Put(TCB::running, tim);
                }
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;
            case(GET_C):

                IOConsole::getCharFromInput();
                __asm__ volatile("sd a0, 80(s0)");
                break;
            case(PUT_C):
                char o;
                __asm__ volatile ("mv %0, a1" : "=r" (o));
                IOConsole::output(o);
                break;
            default:
                break;
        }
        w_sstatus(sstatus);
        w_sepc(new_sepc);

    }
    else if(scause==0x8000000000000001UL)
    {
        // interupt: yes; cause SOFTWARE (ie. Clock timer).

        if(PQS.FirstRelTime() != (time_t)-1)
        {
            PQS.Decrease();
            PQS.WakeFinished();
        }
        mc_sip(SIP_SSIE);
        TCB::timeSliceCounter++;
        if(TCB::timeSliceCounter >= TCB::running->givenTime) {
            TCB::timeSliceCounter = 0;
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
    else if (scause == 0x8000000000000009UL)
    {
//          interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        uint64 volatile new_sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        uint64 reason = plic_claim();
        if (reason == CONSOLE_IRQ) {
            while (CONSOLE_RX_STATUS_BIT & (*(char *) CONSOLE_STATUS)) IOConsole::input(*(char *) CONSOLE_RX_DATA);
        }
        plic_complete((int) reason);
        w_sstatus(sstatus);
        w_sepc(new_sepc);
    }
    else
    {
        // unexpected trap cause
    }

    //w_sepc(new_sepc);

}