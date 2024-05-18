//
// Created by os on 5/13/24.
//

#ifndef TCB_P
#define TCB_P

#include "../lib/hw.h"
#include "scheduler.hpp"

class TCB {
public:
    using Body = void(*)(void *);

    static TCB* running;
    static TCB* createThread(Body body, void* arg);

    bool getFinished() const { return finished; }
    void setFinished(bool val) { finished = val; }

    bool getBlocked() const { return blocked; }
    void setBlocked(bool val) { blocked = val; }

    bool getAsleep() const { return asleep; }
    void setAsleep(bool val) { asleep = val; }

    bool getMode() const { return MODE; }
    void changeMode() { MODE =! MODE; }

    ~TCB() { delete[] stack;}


private:

    TCB(Body body, void* arg):
        body(body),
        arg(arg),
        stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE/8] : nullptr),
        context({
                (uint64) &threadWrapper,
                stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
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
    uint64 *stack;
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
