//
// Created by os on 5/15/24.
//

#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "list.hpp"
#include "tcb.hpp"

// Sem_minor ----Class----
// Semafor core [signal, wait, close, Put, trywait, timedwait, block, unblock, getValue]
class Sem_minor {
public:
    explicit Sem_minor(unsigned int value = 1) :
        value(value),
        closed(false) {};

    static Sem_minor* createSem_minor(Sem_minor** handle, unsigned init);

    void* operator new(size_t size) {
        return MemoryAllocator::mem_alloc((size/MEM_BLOCK_SIZE)+(size%MEM_BLOCK_SIZE!=0 ? 1 : 0));
    }
    void* operator new[](size_t size) {
        return MemoryAllocator::mem_alloc((size/MEM_BLOCK_SIZE)+(size%MEM_BLOCK_SIZE!=0 ? 1 : 0));
    }

    void operator delete(void *ptr) {
        MemoryAllocator::mem_free(ptr);
    }
    void operator delete[](void *ptr) {
        MemoryAllocator::mem_free(ptr);
    }

    ~Sem_minor();

    int signal();
    int wait();
    int close();
    int trywait();
    int timedwait(time_t timeout);

    void block();
    void unblock();

    int getValue() const {return  value;}


private:
    int value;
    bool closed;
    List<TCB> blockedTCBsList;
};


#endif //SEMAPHORE_HPP
