//
// Created by os on 5/18/24.
//

#ifndef PROJECT_BASE_V1_1_PRIORITYQUEUE_MORPHEUS_HPP
#define PROJECT_BASE_V1_1_PRIORITYQUEUE_MORPHEUS_HPP


#include "scheduler.hpp"
#include "tcb.hpp"

class priorityQueueMorpheus
{
private:
    struct Element {
        TCB* thread;
        Element* next;
        time_t relTime;

        void* operator new(size_t size) {
            return MemoryAllocator::mem_alloc(size);
        }
        void* operator new[](size_t size) {
            return MemoryAllocator::mem_alloc(size);
        }

        void operator delete(void *ptr) {
            MemoryAllocator::mem_free(ptr);
        }
        void operator delete[](void *ptr) {
            MemoryAllocator::mem_free(ptr);
        }

        Element(TCB* thread, Element* next, time_t relTime) :
            thread(thread),
            next(next),
            relTime(relTime)
        {} };
    Element *head, *tail;
public:

    priorityQueueMorpheus(): head(nullptr), tail(nullptr){}

    void Decrease(){
        if(head) head->relTime--;
    }
    time_t FirstRelTime(){
        if(!head) return -1;
        return head->relTime;
    }
    void WakeFinished(){
        while(head && head->relTime == 0)
        {
            Element *curr = head;
            head = head->next;
            if(!head) tail = nullptr;
            curr->thread->setAsleep(false);
            Scheduler::put(curr->thread);
            delete curr;
        }
    }
    void Put(TCB* tre, time_t tim){
        if(!head)
        {
            head=new Element(tre, nullptr,tim);
            tail=head;
        }
        else
        {
            Element* curr = head;
            Element* prev = nullptr;
            while(curr && tim >= curr->relTime)
            {
                tim -= curr->relTime;
                prev = curr;
                curr = curr->next;
            }
            if(!curr)
            {
                Element* last = new Element(tre, nullptr,tim);
                tail->next = last;
                tail = last;
            }
            else
            {
                Element* created = new Element(tre,curr,tim);
                if(prev) prev->next = created;
                else head = created;
                curr->relTime -= tim;
            }
        }
    }

};


#endif //PROJECT_BASE_V1_1_PRIORITYQUEUE_MORPHEUS_HPP
