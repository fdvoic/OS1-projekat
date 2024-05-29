//
// Created by os on 5/10/24.
//

#include "../h/error_codes.hpp"
#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"

void* mem_alloc(size_t size){

    size_t sendSize = (size/MEM_BLOCK_SIZE);
    sendSize = sendSize + ((size%MEM_BLOCK_SIZE == 0) ? 0 : 1);

    __asm__ volatile("mv a1, %0" : : "r"(sendSize));
    __asm__ volatile("mv a0, %0" : : "r"(MEM_ALLOC));
    __asm__ volatile("ecall");

    void* pt;
    __asm__ volatile("mv %0, a0" : "=r" (pt));
    return pt;
}

int mem_free(void* pt){

    __asm__ volatile("mv a1, %0" : : "r"(pt));
    __asm__ volatile("mv a0, %0" : : "r"(MEM_FREE));
    __asm__ volatile("ecall");

    uint64 returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return (int)returnValue;

}

int thread_exit(){
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_EXIT));
    __asm__ volatile("ecall");

    uint64 success;
    __asm__ volatile("mv %0, a0" : "=r"(success));
    return (int)success;
}

int thread_create(thread_t *handle, void (*star_routine)(void *), void *arg) {

    void *stack_sp = nullptr;
    if (star_routine) {
        stack_sp = mem_alloc(DEFAULT_STACK_SIZE);
    }

    __asm__ volatile("mv a7, %0" : : "r"(stack_sp));
    __asm__ volatile("mv a6, %0" : : "r"(arg));
    __asm__ volatile("mv a2, %0" : : "r"(star_routine));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_CREATE));
    __asm__ volatile("ecall");

    uint64 success;
    __asm__ volatile("mv %0, a0" : "=r"(success));
    return (int)success;
}

void thread_dispatch() {

    __asm__ volatile("mv a0, %0" : : "r"(THREAD_DISPATCH));
    __asm__ volatile("ecall");
}

int sem_open(sem_t* handle, unsigned init){

    __asm__ volatile ("mv a2, %0" : : "r" (init));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_OPEN));
    __asm__ volatile ("ecall");

    uint64 success;
    __asm__ volatile("mv %0, a0" : "=r"(success));
    return (int)success;

}

int sem_close(sem_t handle) {
    if(!handle) return -2; // ERROR: Argument was incorrect

    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_CLOSE));
    __asm__ volatile ("ecall");

    uint64 success;
    __asm__ volatile("mv %0, a0" : "=r"(success));
    return (int)success;
}

int sem_wait(sem_t id) {
    if(!id) return -2; // ERROR: Argument was incorrect

    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_WAIT));
    __asm__ volatile ("ecall");

    uint64 success;
    __asm__ volatile("mv %0, a0" : "=r"(success));
    return (int)success;
}

int sem_signal(sem_t id) {
    if(!id) return -2; // ERROR: Argument was incorrect

    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_SIGNAL));
    __asm__ volatile ("ecall");

    uint64 success;
    __asm__ volatile("mv %0, a0" : "=r"(success));
    return (int)success;
}

int sem_timedwait(sem_t id, time_t timeout){
    if(!id) return -1;

    __asm__ volatile ("mv a2, %0" : : "r" (timeout));
    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_TIMEDWAIT));
    __asm__ volatile ("ecall");

    uint64 success;
    __asm__ volatile("mv %0, a0" : "=r"(success));
    return (int)success;
}


int sem_trywait(sem_t id) {
    if(!id) return -2; // ERROR: Argument was incorrect

    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_TRYWAIT));
    __asm__ volatile ("ecall");

    uint64 success;
    __asm__ volatile("mv %0, a0" : "=r"(success));
    return (int)success;
}

int time_sleep(time_t tim) {
    if (tim <= 0) return -1;
    __asm__ volatile("mv a1, %0" : : "r"(tim));
    __asm__ volatile("mv a0, %0" : : "r"(TIME_SLEEP));
    __asm__ volatile("ecall");
    return 0;
}

char getc() {

    __asm__ volatile("mv a0,%0"::"r"(GET_C));
    __asm__ volatile("ecall");

    char c;
    __asm__ volatile("mv %0, a0" : "=r"(c));
    return c;
}

void putc(char c) {
    __asm__ volatile("mv a1,%0"::"r"(c));
    __asm__ volatile("mv a0,%0"::"r"(PUT_C));
    __asm__ volatile("ecall");
}
