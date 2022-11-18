#include "./memory.h"

#include "./vm_settings.h"
#include <sys/mman.h>
#include <iostream>

vm::Memory::Memory() {

    stack = mmap(NULL, STACK_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    heap = mmap(NULL, HEAP_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    addressesTable = malloc(ADDRESSES_TABLE_SIZE);
    
    // Zerialize addressTable
    for (address_t _ = 0; _ < ADDRESSES_TABLE_SIZE; _++)
        ((char*)addressesTable)[_] = 0;


    topStackMem = 0; // can go until 0x2000

    // std::cout << stack << std::endl;
    // std::cout << heap << std::endl;

}

address_t vm::Memory::allocStack(address_t __s) { 
    topStackMem += __s;
    if (CHECK_STACK_MEMORY_LIMITS(topStackMem)) 
        { std::cout << "Not enought space to allocate" << std::endl; topStackMem -= __s; return -1; }
    return topStackMem; 
}

address_t vm::Memory::deallocStack(address_t __s) {
    if (__s > topStackMem) __s = topStackMem;
    else if (__s < 0) __s = 0;
    return topStackMem -= __s;
}

void* vm::Memory::getStackAddress(address_t __addr) {
    if (CHECK_STACK_MEMORY_LIMITS(__addr) || __addr > topStackMem) 
        { std::cout << "Invalid stack memory access" << std::endl; exit(1); }// Error stack memory accesss is invalid
    return stack + __addr;
}

bool vm::Memory::addNewAddress(address_t __addr) {

    for (address_t _ = 0; _ < ADDRESSES_TABLE_SIZE; _+=2) {

        if (((address_t*) addressesTable)[_] != 0) continue;

        ((address_t*) addressesTable)[_] = __addr;

        return true;

    }

    return false;

}

address_t vm::Memory::getAddress(uint8_t __pos) { if (__pos > DIFERENT_VAR_NAMES || __pos < 0) return 0; return ((address_t*)addressesTable)[__pos * 2]; }


