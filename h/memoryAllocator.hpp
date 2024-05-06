//
// Created by os on 5/2/24.
//

#ifndef MEMORYALLOCATOR
#define MEMORYALLOCATOR

#include "../lib/hw.h"

// DBlock ----Structure----
// Struktura za vodjenje evidencije o Blokovima podataka.
// Radi preglednijeg koda stoji van klase MemoryAllocator.

struct DBlock {
    DBlock* next;
    DBlock* prev;
    size_t size;
};



// MemoryAllocator ----Singleton Class----
// Upravljanje memorijom [Alloc, Free, Initialize]
class MemoryAllocator {
public:

    static DBlock* hUsed;
    static DBlock* hFree;


    static void* mem_alloc(size_t size);
    static int mem_free(void*);

    // Pokusava spajanje FREE segmenta [curr] sa [curr->next].
    // Slicna implementacija kao sa zadatka radjenog na vezbama.
    static void tryToJoin(DBlock* curr);

    // Kernel poziva pri pokretanju.
    static void InitializeHeap();

};

#endif //MEMORYALLOCATOR
