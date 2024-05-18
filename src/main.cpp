
#include "../h/memoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../javniTestovi_2024_1_1/userMain.hpp"
#include "../h/sem_minor.hpp"
//#include "../javniTestovi_2024_1_1/printing.hpp"



void omotac(void*){
    //Riscv::mc_sstatus(Riscv::SSTATUS_SPIE);
    //Riscv::popSppSpie();
    userMain();
    Riscv::USER_END = true;
}

void main(){

    MemoryAllocator::InitializeHeap();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB* threadsa[5];
    thread_create(&threadsa[0], nullptr, nullptr);
    threadsa[0]->changeMode();

    //char* arg_b=(char*)"Proveri parametar B";
    //thread_create(&threadsa[1], workerBodyA, (void*) arg_b);
    //threadsa[0]=createThread(nullptr, nullptr);


    TCB::running = threadsa[0];


    //Sem_minor* semafor = nullptr;
    //sem_open(&semafor,1);
    //sem_close(semafor);

    //sem_signal(semafor);
    //sem_wait(semafor);
    // __putc('5' + semafor->getValue());


    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_create(&threadsa[1], &omotac, nullptr);
    Riscv::USER_END = false;
    while(!Riscv::USER_END)
    {
        thread_dispatch();
    }

    return;
}