//
// Created by os on 5/12/24.
//

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler {
public:

    static TCB *get();
    static void put(TCB *tcb);

private:

    static List<TCB> readyThreadQueue;

};


#endif //SCHEDULER_HPP
