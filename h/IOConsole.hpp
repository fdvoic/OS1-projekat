//
// Created by os on 5/19/24.
//

#ifndef IOCONSOLE_HPP
#define IOCONSOLE_HPP

#include "Console_buffer.hpp"


class Console_buffer;

// IOConsole ----Singleton Class----
// Konzola [getChar, putChar, getAVL]
class IOConsole {
public:
    static char getCharFromInput();
    static char getCharFromOutput();

    static void input(char c);
    static void output(char c);


    static bool Flushed() {return outputStream->getAVL();}

    static void InitializeIOConsole();

private:

    IOConsole() = default;
    ~IOConsole() = default;

    void* operator new (size_t size) {
        return MemoryAllocator::mem_alloc((size/MEM_BLOCK_SIZE)+(size%MEM_BLOCK_SIZE!=0 ? 1 : 0));
    }


    void operator delete(void* ptr){
        MemoryAllocator::mem_free(ptr);
    }

    static Console_buffer* inputStream;
    static Console_buffer* outputStream;
};


#endif //IOCONSOLE_HPP
