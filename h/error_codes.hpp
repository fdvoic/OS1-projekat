//
// Created by os on 5/10/24.
//

#ifndef PR_ERROR_CODES_HPP
#define PR_ERROR_CODES_HPP

#include "../lib/hw.h"


constexpr uint64 MEM_ALLOC = 0x01;
constexpr uint64 MEM_FREE = 0x02;

constexpr uint64 THREAD_CREATE= 0x11;
constexpr uint64 THREAD_EXIT = 0x12;
constexpr uint64 THREAD_DISPATCH = 0x13;

constexpr uint64 SEM_OPEN = 0x21;
constexpr uint64 SEM_CLOSE = 0x22;
constexpr uint64 SEM_WAIT = 0x23;
constexpr uint64 SEM_SIGNAL = 0x24;
constexpr uint64 SEM_TIMEDWAIT = 0x25;
constexpr uint64 SEM_TRYWAIT = 0x26;

constexpr uint64 TIME_SLEEP = 0x31;

constexpr uint64 GET_C = 0x41;
constexpr uint64 PUT_C = 0x42;


constexpr int SEMDEAD = -1;
constexpr int TIMEOUT = -2;




#endif //PR_ERROR_CODES_HPP
