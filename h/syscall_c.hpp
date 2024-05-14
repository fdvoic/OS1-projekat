//
// Created by os on 5/2/24.
//

#ifndef SYSCALL_C
#define SYSCALL_C

#include "../lib/hw.h"

// MemoryAllocator
void* mem_alloc (size_t size); //0x01
int mem_free(void*); //0x02


// Thread
class TCB;
typedef TCB* thread_t;

int thread_create(thread_t* handle, void (*star_routine)(void*),void* arg); //0x11
int thread_exit(); //0x12
void thread_dispatch(); //0x13



//Semaphore
class _sem;
typedef _sem* sem_t;

//Console


#endif //SYSCALL_C_HPP
