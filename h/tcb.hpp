//
// Created by os on 5/13/24.
//

#ifndef TCB_P
#define TCB_P

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "syscall_c.hpp"

class TCB {
public:
    using Body = void(*)(void *);

    static TCB* running;
    static TCB* createThread(Body body, void* arg, void* stack);

    bool getFinished() const { return finished; }
    void setFinished(bool val) { finished = val; }

    bool getBlocked() const { return blocked; }
    void setBlocked(bool val) { blocked = val; }

    bool getAsleep() const { return asleep; }
    void setAsleep(bool val) { asleep = val; }

    bool getMode() const { return MODE; }
    void changeMode() { MODE =! MODE; }

    ~TCB() { MemoryAllocator::mem_free(stack);}


private:

    TCB(Body body, void* arg, void* st):
        body(body),
        arg(arg),
        stack(st),
        context({
                (uint64) &threadWrapper,
                (uint64) stack + DEFAULT_STACK_SIZE - 1
        }),
        finished(false),
        blocked(false),
        asleep(false),
        givenTime(DEFAULT_TIME_SLICE),
        MODE(false)
        {
            if(body != nullptr) {Scheduler::put(this);}
        }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    void *stack;
    Context context;
    bool finished;
    bool blocked;
    bool asleep;
    uint64 givenTime;
    bool MODE;

    friend class Riscv;
    friend class Sem_minor;

    static void threadWrapper();
    static void contextSwitch(Context *oldContext, Context *newContext);
    static void dispatch();

    static uint64 timeSliceCounter;

};


#endif //TCB_P
