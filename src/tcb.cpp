//
// Created by os on 5/13/24.
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

TCB* TCB::running = nullptr;

TCB *TCB::createThread(Body body, void* arg) {
    return new TCB(body,arg);

}

void TCB::threadWrapper() {
    //Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    dispatch();
}

void TCB::dispatch() {
    TCB* old = running;
    if(!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();
    if(old != running) contextSwitch(&old->context, &running->context);
}
