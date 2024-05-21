//
// Created by os on 5/20/24.
//

#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"

//==================Console===========================

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

//==================Semaphore===========================

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}




int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
    mem_free(myHandle);
}



//===================Thread===========================



Thread::Thread(void (*body)(void *), void *arg)  : body(body), arg(arg) {
    myHandle=nullptr;
}

Thread::Thread() : Thread(&threadWrapper,this) {
    //thread_create(&myHandle, &threadWrapper, this);
}

int Thread::start() {
    if (myHandle==nullptr) {
        thread_create(&myHandle,body,arg);
        return 0;
    }
    else return -1;
}

void Thread::threadWrapper(void *tr) {
    if(tr)((Thread*)tr)->run();
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t tim) {
    return time_sleep(tim);
}




Thread::~Thread() {
   myHandle->setFinished(true);
   delete myHandle;
}