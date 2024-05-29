//
// Created by os on 5/15/24.
//

#include "../h/riscv.hpp"
#include "../h/sem_minor.hpp"
#include "../h/error_codes.hpp"

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
    if(ready->getAsleep()){
        Riscv::PQS.RemoveSpecific(ready);  //REMOVE FROM LIST OF SLEEPING TCBS
        ready->setAsleep(false);
    }
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

int Sem_minor::timedwait(time_t timeout) {
    if(trywait() == 1) return 0;
    TCB::running->setAsleep(true);
    Riscv::PQS.Put(TCB::running, timeout);
    --this->value;
    block();
    if(closed) return SEMDEAD;
    if(TCB::running->getFailedSemTimedWait())
    {
        TCB::running->setFailedSemTimedWait(false);
        return TIMEOUT;
    }
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



