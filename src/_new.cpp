//
// Created by os on 5/12/24.
//

#include "../h/syscall_c.hpp"

using size_t = decltype(sizeof(0));

void *operator new(size_t size)
{
    return mem_alloc(size);
}

void *operator new[](size_t size)
{
    return mem_alloc(size);
}

void operator delete(void* ptr) noexcept
{
    mem_free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    mem_free(ptr);
}
