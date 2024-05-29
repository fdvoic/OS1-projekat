//
// Created by os on 5/19/24.
//

#ifndef CONSOLE_BUFFER_HPP
#define CONSOLE_BUFFER_HPP

#include "sem_minor.hpp"

// Console_buffer ----Class----
// Buffer za char-ove [getChar, putChar, getAVL]

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
        buffer=(char*)MemoryAllocator::mem_alloc(((size*sizeof(char))/MEM_BLOCK_SIZE)+((size*sizeof(char))%MEM_BLOCK_SIZE!=0 ? 1 : 0));
        Sem_minor::createSem_minor(&item,0);
        Sem_minor::createSem_minor(&space,size);
    }


    char getChar();
    void putChar(char c);

    bool getAVL() const {return (uint64)space->getValue() == size;}

    void* operator new (size_t size) {
        return MemoryAllocator::mem_alloc((size/MEM_BLOCK_SIZE)+(size%MEM_BLOCK_SIZE!=0 ? 1 : 0));
    }

    void operator delete(void* ptr){

        MemoryAllocator::mem_free(ptr);

    }

};


#endif //CONSOLE_BUFFER_HPP
