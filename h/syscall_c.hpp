//
// Created by os on 5/2/24.
//

#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

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
class Sem_minor;
typedef Sem_minor* sem_t;

int sem_open(sem_t* handle, unsigned init); //0x21
int sem_close(sem_t handle); //0x22
int sem_wait(sem_t id); //0x23
int sem_signal(sem_t id); //0x24
int sem_timedwait(sem_t id, time_t timeout); //0x25
int sem_trywait(sem_t id); //0x26


// Time
typedef unsigned long time_t;
int time_sleep (time_t);


//Console
const int EOF = -1;

char getc ();
void putc (char);

#endif //SYSCALL_C_HPP
