#include "./commonFunctions.h"

#include <iostream>

int utils::getWordSize(char* _) {

    int _rtr = 0;

    while(*(_++) != 32) _rtr++;

    return _rtr;

}

bool utils::isInt(char* _) {

    while(*_) {

        if (
            (*_ < 48 || *_ > 57) && *(_) != 95
        ) return false;

        _++;
        
    }

    return true;

}

bool utils::isLong(char*) {

    return false;

}

bool utils::isDouble(char*) {

    return false;

}

int utils::getStringSize(char* __d) {

    int _ = 0;

    do _++;
    while(*(__d++));

    return _;

}

char* utils::copyString(char* __d, int __s) {

    char* _ = (char*) malloc(__s);

    for (int __ = 0; __ < __s; __++) _[__] = __d[__];

    return _; 

}




