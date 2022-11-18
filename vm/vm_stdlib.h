#pragma once

#ifndef VM_STDLIB_H
#define VM_STDLIB_H

#include <stdint.h>

typedef uint16_t address_t;

// Forward
namespace vm { struct Memory; }

namespace vm_stdlib {


    /* Memory management */

    // Stack

    /* Allocates a specific size into stack and set given data into
    *   Return address of variable 
    */
    address_t allocMemStack(void*, int, vm::Memory*);
    /* Deallocates a specefic size of stack
    *   @param __ma Memory Address to where it should dealloc
    */
    address_t deallocMemStack(int, vm::Memory*);

    /* Operations */

    /* Performs a binary addition and stores in given address 
    *   @param  __f Address of first value
    *   @param  __sf Size of first variable 
    *   @param  __s Address of second value
    *   @param  __ss Size of first variable 
    *   @param  __r Address to store the result
    *   it assumes that __r sizes is equal to __f
    */
    void binaryAddition(void*, int, void*, int, void*);

}

#endif