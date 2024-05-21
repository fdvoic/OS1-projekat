//
// Created by os on 5/15/24.
//

#include "../h/riscv.hpp"
#include "../h/sem_minor.hpp"
#include "../h/tcb.hpp"
#include "../h/syscall_c.hpp"

Sem_minor *Sem_minor::createSem_minor(Sem_minor **handle, unsigned int init) {
    *handle = new Sem_minor(init);
    return *handle;
}

void Sem_minor::block() {
    TCB::running->setBlocked(true);
    blockedTCBsList.addLast(TCB::running);
    TCB::dispatch();
}

void Sem_minor::unblock() {
    TCB* ready = blockedTCBsList.removeFirst();
    ready->setBlocked(false);
    Scheduler::put(ready);
}

int Sem_minor::wait() {
    if(closed) return -1;
    --this->value;
    if((int)this->value < 0)
    {
        block();
        if(closed) return -1; // I returned because my semaphore was closed.
    }
    return 0;
}

int Sem_minor::signal() {
    if(closed) return -1;
    ++this->value;
    if((int) this->value <= 0) unblock();
    return 0;
}

int Sem_minor::trywait() {
    if(closed) return -1;

    int tmp = value - 1;
    if (tmp < 0) return 0;
    this->value--;
    return 1;
}


int Sem_minor::close() {
    if(closed) return -1; // Should not happen, right?
    closed = true;
    while(blockedTCBsList.peekFirst())  // Unblock all.
    {
        TCB* ready = blockedTCBsList.removeFirst();
        ready->setBlocked(false);
        Scheduler::put(ready);
    }
    return 0;
}

Sem_minor::~Sem_minor() {
    close();
}



