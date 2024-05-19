//
// Created by os on 5/19/24.
//

#include "../h/Console_buffer.hpp"


char Console_buffer::getChar() {
    item->wait();
    char c = buffer[head];
    head = (head+1)%size;
    space->signal();
    return c;
}


void Console_buffer::putChar(char c) {
    space->wait();
    buffer[tail] = c;
    tail = (tail+1)%size;
    item->signal();
}