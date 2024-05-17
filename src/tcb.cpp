//
// Created by os on 5/13/24.
//
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"

TCB* TCB::running = nullptr;

TCB *TCB::createThread(Body body, void* arg) {
    return new TCB(body,arg);

}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    dispatch();
}

void TCB::dispatch() {
    //int imp = Riscv::r_sstatus(); // Soemthing is wrong with sret :^[

    TCB* old = running;
    if(!old->getFinished() && !old->getBlocked()) { Scheduler::put(old); }
    running = Scheduler::get();

    //Riscv::w_sstatus(imp); // Ummmmmmmm necessary?

    // running == nullptr     <=>    Kernel tried sepuku. Should NOT happen.
    if(old != running && running != nullptr) contextSwitch(&old->context, &running->context);
}
