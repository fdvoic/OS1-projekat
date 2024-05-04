//
// Created by os on 5/2/24.
//

#include "../lib/hw.h"
#include "../h/MemAllocator.hpp"

DBlock* MemoryAllocator::hUsed=nullptr;
DBlock* MemoryAllocator::hFree=nullptr;





void MemoryAllocator::tryToJoin(DBlock* curr){
    if(!curr) return;
    if(curr->next && (char*)curr+curr->size+sizeof(DBlock) == (char*) curr->next){
        curr->size += curr->next->size + sizeof(DBlock);
        curr->next = curr->next->next;
        if(curr->next) curr->next->prev=curr;
    }
}

void MemoryAllocator::InitializeHeap(){

}



