//
// Created by os on 5/2/24.
//

#ifndef SYSCALL_C
#define SYSCALL_C

#include "../lib/hw.h"

//MemoryAlloc
void* mem_alloc (size_t size); //0x01
int mem_free(void*); //0x02

//Thread
class _thread;
typedef _thread* thread_t;

int thread_create(thread_t* handle, void (*star_routine)(void*),void* arg); //0x11
int thread_exit(); //0x12
void thread_dispatch(); //0x13


//Semaphore


//Console


#endif //SYSCALL_C_HPP
