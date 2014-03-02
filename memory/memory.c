#include <stdint.h>
#include <stdio.h>
#include "memory.h"
#include "../util/util.h"
#include "../include/constants.h"


void *start_address;
static MemoryManager *mmr = 0;

uint32_t getCurrentPID(void) {
    return 0;
}

void set_start_address(void *addr) {
    start_address = addr;
}

void *myMalloc(unsigned int size) {

    Region *r;
    if (mmr == 0) {
        mmr = initialize_memory(start_address, TOTAL_SPACE);
    }

    if (size == 0) {
        return 0;
    }

    size = adjust_size(size);
    if (cannot_allocate(mmr, size)) {
        return 0;
    }
    r = allocate_region(mmr, size);
    reduce_available_space(mmr, size);
    return r->data;
}

Region *allocate_region(MemoryManager *mmr, unsigned int size) {
    Region *r = mmr->leading_edge;
    r->size = size;
    r->free = 0;
    return r;
}

void reduce_available_space(MemoryManager *mmr, unsigned int size) {
    uintptr_t shift = (uintptr_t) mmr->leading_edge;
    mmr->remaining_space -= size;
    shift = shift + size + sizeof(Region);
    mmr->leading_edge = (Region *) shift;
}

Region *region_for_pointer(void *ptr) {
    return ((Region *) ptr - 1);
}

MemoryManager *initialize_memory(void *start_address,
                                unsigned long total_space) {
    MemoryManager *mmr = (MemoryManager *) start_address;
    mmr->remaining_space = total_space - (sizeof(MemoryManager) + sizeof(Region));
    mmr->base_region = (Region *) ((uintptr_t) start_address + sizeof(MemoryManager));
    mmr->base_region->free = 1;
    mmr->base_region->size = 0;
    mmr->leading_edge = mmr->base_region;
    return mmr;
}

unsigned int adjust_size(unsigned int size) {
    unsigned int double_word_size = 2 * WORD_SIZE;
    unsigned int padding = double_word_size - 1;
    return (size + padding) & ~padding;
}

unsigned int remaining_space(MemoryManager *mmr) {
    return mmr->remaining_space;
}

int cannot_allocate(MemoryManager *mmr, unsigned int size) {
    if ((size + sizeof(Region)) > mmr->remaining_space) {
        return TRUE;
    } else {
        return FALSE;
    }
}


void myFree(void *ptr) {
    if (mmr == 0) {
        mmr = initialize_memory(start_address, TOTAL_SPACE);
    }

    if (ptr == 0) {
        return;
    }

    Region *cursor = mmr->base_region;
    //Region *r = region_for_pointer(ptr);
    //r->free = 1;
}