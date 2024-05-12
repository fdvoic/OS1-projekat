//
// Created by os on 5/10/24.
//

#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"

void* mem_alloc(size_t size){

    size_t sendSize = (size/MEM_BLOCK_SIZE);
    sendSize = sendSize + ((size%MEM_BLOCK_SIZE == 0) ? 0 : 1);

    __asm__ volatile("mv a1, %0" : : "r"(sendSize));
    __asm__ volatile("li a0, 0x01");
    __asm__ volatile("ecall");

    void* pt;
    __asm__ volatile("mv %0, a0" : "=r" (pt));
    return pt;
}

int mem_free(void* pt){

    __asm__ volatile("mv a1, %0" : : "r"(pt));
    __asm__ volatile("li a0, 0x02");
    __asm__ volatile("ecall");

    uint64 returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return (int)returnValue;

}