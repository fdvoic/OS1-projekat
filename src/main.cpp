
#include "../h/memoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../javniTestovi_2024_1_1/userMain.hpp"
#include "../h/sem_minor.hpp"
//#include "../javniTestovi_2024_1_1/printing.hpp"

static volatile bool finishedA = false;
static volatile bool finishedB = false;



static void workerBodyA(void* arg) {
    for (uint64 i = 0; i < 10; i++) {

        for (uint64 j = 0; j < 1000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            //thread_dispatch();
        }
        *(char*)arg+=1;
    }
    finishedA = true;
    __putc('A');
    __putc(*(char*)arg);
    __putc('\n');
}

static void workerBodyB(void* arg) {
    for (uint64 i = 0; i < 10; i++) {

        for (uint64 j = 0; j < 1000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    finishedB = true;
    __putc('B');
    //thread_exit();
    __putc(*((char*)arg+10));
    __putc('\n');
}


void omotac(){
    thread_t threads[4];
    char* arg_a=new char;
    char* arg_b=(char*)"Proveri parametar B";
    *arg_a='A';
    thread_create(&threads[0], workerBodyA, (void*)arg_a);
    thread_create(&threads[1], workerBodyB, (void*)arg_b);
    while (!(finishedA && finishedB)) {
        thread_dispatch();
    }
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


    TCB* threadsa[5];

    thread_create(&threadsa[0], nullptr, nullptr);

    //char* arg_b=(char*)"Proveri parametar B";
    //thread_create(&threadsa[1], workerBodyA, (void*) arg_b);
    //threadsa[0]=createThread(nullptr, nullptr);


    TCB::running = threadsa[0];


    //Sem_minor* semafor = nullptr;
    //sem_open(&semafor,1);
    //sem_close(semafor);

    //sem_signal(semafor);
    //sem_wait(semafor);
    // __putc('5' + semafor->getValue());


    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    userMain();


    /*
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
    */
    return;
}