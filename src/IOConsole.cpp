//
// Created by os on 5/19/24.
//

#include "../h/IOConsole.hpp"


Console_buffer* IOConsole::inputStream;
Console_buffer* IOConsole::outputStream;

char IOConsole::getCharFromInput() {
    return inputStream->getChar();
}

char IOConsole::getCharFromOutput() {
    return outputStream->getChar();
}

void IOConsole::input(char c) {
    inputStream->putChar(c);
}

void IOConsole::output(char c) {
    outputStream->putChar(c);
}

void IOConsole::InitializeIOConsole() {
    inputStream = new Console_buffer(2048);
    outputStream = new Console_buffer(2048);
}
