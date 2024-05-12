
#include "../h/memoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../h/riscv.hpp"

extern "C" void supervisorTrap();

void omotac(){
    //__asm__ volatile ("mv t0, ra");
    uint64 volatile x;
    __asm__ volatile ("mv %0, ra" : "=r"(x));
    __asm__ volatile("csrw sepc, %0" : : "r" (x));
    //__asm__ volatile ("csrw t0, sepc");
}

void main(){

    //uint64 OLD_VEC = Riscv::r_stvec();
    MemoryAllocator::InitializeHeap();
    //__putc(HEAP_END_ADDR);
    //void* p = MemoryAllocator::mem_alloc(100);
    //MemoryAllocator::mem_free(p);
    //MemoryAllocator::mem_alloc(100000);
    //MemoryAllocator::mem_free((void*)(0x80005608-0x18));
    //(void*)(0x80006509)

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);


    int* a = (int*) mem_alloc(200);
    int* b = (int*) mem_alloc(254);
    int* c = (int*) mem_alloc(32);
    int uspeh=mem_free((void*)b);
    int* f = (int*) mem_alloc(257);
    a++;
    b++;
    c++;
    uspeh++;
    f++;
    return;
}