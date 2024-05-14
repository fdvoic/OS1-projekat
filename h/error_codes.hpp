//
// Created by os on 5/10/24.
//

#ifndef PR_ERROR_CODES
#define PR_ERROR_CODES

#include "../lib/hw.h"


constexpr uint64 MEM_ALLOC = 0x01;
constexpr uint64 MEM_FREE = 0x02;
constexpr uint64 THREAD_CREATE= 0x11;
constexpr uint64 THREAD_EXIT = 0x12;
constexpr uint64 THREAD_DISPATCH = 0x13;




#endif //PR_ERROR_CODES
