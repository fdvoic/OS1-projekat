//
// Created by os on 5/13/24.
//
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;


TCB *TCB::createThread(Body body, void* arg, void* stack) {
    return new TCB(body,arg,stack);

}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
    //running->setFinished(true);
    //dispatch();
    //thread_dispatch();
}

void TCB::dispatch() {

    TCB* old = running;
    if(!old->getFinished() && !old->getBlocked() && !old->getAsleep()) { Scheduler::put(old); }
    running = Scheduler::get();
    //TCB::timeSliceCounter = 0;

    // running == nullptr     <=>    Kernel je pokusao seppuku. Should NOT happen.
    if(old != running && running != nullptr) contextSwitch(&old->context, &running->context);
}
