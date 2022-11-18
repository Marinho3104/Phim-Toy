#include "./vm_stdlib.h"


#include "./../utils/byteOperations.h" // Binary operations
#include "./memory.h" // Memory

#include <iostream>
#include <string.h>

address_t vm_stdlib::allocMemStack(void* __d, int __s, vm::Memory* __mem) {
    address_t _stackAddr = __mem->allocStack(__s) - __s + 1;
    if (_stackAddr == -1 || !__d) return _stackAddr;
    void* _addr = __mem->getStackAddress(_stackAddr);
    for (int _ = 0; _ < __s; _++) ((unsigned char*) _addr)[_] = ((unsigned char*) __d)[_];
    return _stackAddr;
}

address_t vm_stdlib::deallocMemStack(int __ma, vm::Memory* __mem) { __mem->deallocStack(__mem->topStackMem - __ma); }

/*  Binary Operations */

void vm_stdlib::binaryAddition(void* __f, int __fs, void* __s, int __ss, void* __r) {
    bool _ovr = false;
    int _ = 0;
    for (; __fs > _ && __ss > _; _++) 
        ((unsigned char*)__r)[_] = utils::addBytes(
            ((unsigned char*) __f)[_],
            ((unsigned char*) __s)[_],
            _ovr
        );
        
    // for (; __fs > 0; __fs--) ((unsigned char*)__r)[__fs - 1] = ((unsigned char*) __f)[__fs - 1];
}

