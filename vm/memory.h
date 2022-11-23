#pragma once

#ifndef VM_MEMORY_H
#define VM_MEMORY_H

#include "./vm_definitions.h" // Vm Definitions

namespace vm {

    /* Emulates memory needed to execute a program (a.k.a contract) in VM
    *
    *   Stack memory
    *   Heap memory
    * 
    */
    struct Memory {

        void* stack, *heap, *addressesTable;
        /* As in a real machine execution this indicates the top address of stack
        *   Any access to stack greater than this value is not  possible 
        */
        address_t topStackMem; 

        /* Deallocates all memory use */
        ~Memory() = default;

        /* Allocates all needed memory to use */
        Memory();

        /* Allocates memory in stack and return current top stack address 
        *   If dont have enought space to allocate and dont allocate any memory return -1
        *   @param __s Bytes size of allocation
        */
        address_t allocStack(address_t);
        /* Deallocates memory in stack and return current top stack address 
        *   If given size greater than bytes allocated it will just deallocates currnt bytes allocated
        *   @param __s Bytes size of deallocation
        */
        address_t deallocStack(address_t);
        /* Get the "real" address in memory of given address
        *   
        */
        void* getStackAddress(address_t);
        /* Add new address to table 
        *   Address are store in concurrency? which means when trying to access address 0 means trying to access addressesTable[0]
        *   all mapping of variables names is in charge of execution
        */
        bool addNewAddress(address_t);
        /**/
        address_t getAddress(uint8_t);

    };

}

#endif