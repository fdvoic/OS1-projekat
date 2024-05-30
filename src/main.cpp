
#include "../h/memoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"
#include "../javniTestovi_2024_1_1/userMain.hpp"
#include "../h/sem_minor.hpp"
#include "../h/IOConsole.hpp"

void ConsoleOutputStreamWrapper(void*) {
    while (1) {
        volatile char status = *((char *) CONSOLE_STATUS);
        while ((status & CONSOLE_TX_STATUS_BIT))
        {
            char c = IOConsole::getCharFromOutput();
            *((char *) CONSOLE_TX_DATA) = c;
            status = *((char *) CONSOLE_STATUS);
        }
    }
}

void LOOP(void *)
{
    while(1) thread_dispatch();
}

void UserMainWrapper(void*){
    userMain();
}

void main(){


    MemoryAllocator::InitializeHeap();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    IOConsole::InitializeIOConsole();

    TCB* SYS_THREADS[3];
    TCB* USER_THREADS[1];

    thread_create(&SYS_THREADS[0], nullptr, nullptr);
    SYS_THREADS[0]->changeMode();
    TCB::running = SYS_THREADS[0];

    thread_create(&SYS_THREADS[1],ConsoleOutputStreamWrapper, nullptr);
    SYS_THREADS[1]->changeMode();

    thread_create(&SYS_THREADS[2],LOOP, nullptr);
    SYS_THREADS[2]->changeMode();

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    thread_create(&USER_THREADS[0], &UserMainWrapper, nullptr);

    while(!USER_THREADS[0]->getFinished())
    {
        thread_dispatch();
    }
    while (!IOConsole::Flushed())  thread_dispatch();


    return;
}