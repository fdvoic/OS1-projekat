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

int thread_create(thread_t *handle, void (*star_routine)(void *), void *arg) {

    __asm__ volatile("mv a7, %0" : : "r"(arg));
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
