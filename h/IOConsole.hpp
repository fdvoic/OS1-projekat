//
// Created by os on 5/19/24.
//

#ifndef PROJECT_BASE_V1_1_IOCONSOLE_HPP
#define PROJECT_BASE_V1_1_IOCONSOLE_HPP

#include "Console_buffer.hpp"

class Console_buffer;
class IOConsole {
public:
    static char getCharFromInput();
    static char getCharFromOutput();

    static void input(char c);
    static void output(char c);


    static bool Flushed() {return outputStream->getAVL();}

    static void InitializeIOConsole();

private:

    void* operator new (size_t size) {
        return MemoryAllocator::mem_alloc(size);
    }


    void operator delete(void* ptr){
        MemoryAllocator::mem_free(ptr);
    }

    static Console_buffer* inputStream;
    static Console_buffer* outputStream;
};


#endif //PROJECT_BASE_V1_1_IOCONSOLE_HPP
