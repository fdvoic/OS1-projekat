//
// Created by os on 5/2/24.
//

#include "../lib/hw.h"
#include "../h/memoryAllocator.hpp"

DBlock* MemoryAllocator::hUsed=nullptr;
DBlock* MemoryAllocator::hFree=nullptr;

const uint64 SizeDBlock=sizeof(DBlock);

void* MemoryAllocator::mem_alloc(size_t size) {
    size_t tempSize=size * MEM_BLOCK_SIZE;
    // Ovaj sledeci deo ne bi trebao da bude potreban,
    // jer on bi trebao da bude obradjen u C API-u  ---> ABI
    //if(tempSize % MEM_BLOCK_SIZE != 0) tempSize=((tempSize + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE) * MEM_BLOCK_SIZE;

    DBlock* curr=hFree;
    for(; curr; curr=curr->next) {
        if(curr->size >= tempSize) break;
    }

    if(curr == nullptr) return nullptr; // Nema slobdonog/dovoljnog mesta.
    else {
        // Priprema za obradu.
        if(curr->size == tempSize || curr->size - tempSize - SizeDBlock <= 0) {
            // Velicina je savrseno poklopljena
            // ILI nema dovoljno prostora da napravi novi Free segment (nastaje mala [sizeof(DBlock)] interna fragmentacija).
            if(curr->prev) curr->prev->next = curr->next;
            else hFree = curr->next;

            if(curr->next) curr->next->prev = curr->prev;
        }
        else {
            DBlock* newDBlock = (DBlock*) ((char*) curr + tempSize + SizeDBlock);

            if(curr->prev) curr->prev->next = newDBlock;
            else hFree = newDBlock;

            if(curr->next) curr->next->prev = newDBlock;
            newDBlock->prev = curr->prev;
            newDBlock->next = curr->next;

            newDBlock->size = curr->size - tempSize - SizeDBlock; // NE SME BITI NEGATIVAN.
            curr->size=tempSize;
        }

        if(hUsed == nullptr) {
            hUsed = curr;
            hUsed->next = nullptr;
            hUsed->prev = nullptr;
        }
        else if((char*) curr < (char*) hUsed) {
            hUsed->prev = curr;
            curr->prev = nullptr;
            curr->next = hUsed;
            hUsed = curr;
        }
        else {
            DBlock* temp = hUsed;
            for(; temp->next && (char*)(temp->next) < (char*) curr; temp = temp->next);

            curr->next = temp->next;
            curr->prev = temp;
            if(curr->next) curr->next->prev = curr;
            temp->next = curr;
        }
        return (char*) curr + SizeDBlock;
    }
}

int MemoryAllocator::mem_free(void* segment) {

    if(!hUsed || !segment) return -1;
    if(segment < HEAP_START_ADDR || segment >= HEAP_END_ADDR) return -2;

    DBlock* remUsed = (DBlock*)((char*) segment - SizeDBlock);
    if(remUsed < hUsed) return -2;

    if(remUsed == hUsed) {
        hUsed=hUsed->next;
        if(hUsed) hUsed->prev= nullptr;
    }
    else {
        remUsed->prev->next=remUsed->next;
        if(remUsed->next) remUsed->next->prev=remUsed->prev;
    }
    remUsed->next = nullptr;
    remUsed->prev = nullptr;

    // Ovo se moze desiti SAMO u slucaju,
    // kada ostane poslednje parce koje je neiskorisceno.
    if(hFree == nullptr) hFree=remUsed;
    else if((char*) remUsed < (char*)hFree) {
        hFree->prev=remUsed;
        remUsed->next=hFree;
        hFree=remUsed;
        tryToJoin(hFree);
    }
    else {
        DBlock* temp=hFree;
        for(; temp->next && (char*)(temp->next)< (char*) remUsed; temp=temp->next);

        remUsed->next= temp->next;
        remUsed->prev= temp;
        if(temp->next) temp->next->prev=remUsed;
        temp->next=remUsed;
        tryToJoin(remUsed);
        tryToJoin(temp);
    }
    return 0;
}


void MemoryAllocator::tryToJoin(DBlock* curr){
    if(!curr) return;
    if(curr->next && (char*)curr+curr->size+SizeDBlock == (char*) curr->next){
        curr->size += curr->next->size + SizeDBlock;
        curr->next = curr->next->next;
        if(curr->next) curr->next->prev=curr;
    }
}

void MemoryAllocator::InitializeHeap(){
    hFree=(DBlock*)((char*) HEAP_START_ADDR);
    hUsed=nullptr;

    hFree->next=nullptr;
    hFree->prev=nullptr;

    hFree->size=((char*) HEAP_END_ADDR - (char*) HEAP_START_ADDR - SizeDBlock);
}



