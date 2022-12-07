#include "./parser_helper.h"

#include <iostream>

bool parser::isInt(char* __to_check) {

    while(*__to_check) {

        if (
            (*__to_check < 48 || *__to_check > 57) && *(__to_check) != 95
        ) return false;

        __to_check++;
        
    }

    return true;

}

