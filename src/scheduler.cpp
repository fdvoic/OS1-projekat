//
// Created by os on 5/12/24.
//

#include "../h/scheduler.hpp"

List<TCB> Scheduler::readyThreadQueue;

TCB* Scheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *tcb)
{
    readyThreadQueue.addLast(tcb);
}