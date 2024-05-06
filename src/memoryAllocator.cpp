//
// Created by os on 5/2/24.
//

#include "../lib/hw.h"
#include "../h/memoryAllocator.hpp"

DBlock* MemoryAllocator::hUsed=nullptr;
DBlock* MemoryAllocator::hFree=nullptr;

const uint64 SizeDBlock=sizeof(DBlock);




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



}



