//
// Created by os on 5/2/24.
//

#ifndef MEMALLOCATOR
#define MEMALLOCATOR

#include "../lib/hw.h"

// DBlock ----Structure----
// Struktura za vodjenje evidencije o Blokovima podataka.
// Radi preglednijeg koda stoji van klase MemoryAllocator.
struct DBlock {
    size_t size;
    DBlock* next;
    DBlock* prev;
};



// MemoryAllocator ----Singleton Class----
// Upravljanje memorijom [Alloc, Free, Initialize]
class MemoryAllocator{
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

#endif //MEMALLOCATOR
