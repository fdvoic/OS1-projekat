//
// Created by os on 5/19/24.
//

#ifndef PROJECT_BASE_V1_1_CONSOLE_BUFFER_HPP
#define PROJECT_BASE_V1_1_CONSOLE_BUFFER_HPP

#include "sem_minor.hpp"

class Console_buffer {
private:
    char* buffer;
    uint64 head;
    uint64 tail;
    uint64 size;
    Sem_minor* item, *space;

public:

    Console_buffer(uint64 size) : head(0), tail(0), size(size)
    {
        buffer=(char*)MemoryAllocator::mem_alloc(size*sizeof(char));
        Sem_minor::createSem_minor(&item,0);
        Sem_minor::createSem_minor(&space,size);
    }


    char getChar();
    void putChar(char c);

    bool getAVL() const {return (uint64)space->getValue() == size;}

    void* operator new (size_t size) {
        return MemoryAllocator::mem_alloc(size);
    }

    void operator delete(void* ptr){

        MemoryAllocator::mem_free(ptr);

    }

};


#endif //PROJECT_BASE_V1_1_CONSOLE_BUFFER_HPP
