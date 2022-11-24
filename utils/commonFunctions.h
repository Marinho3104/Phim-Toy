#pragma once

#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <iostream>

namespace utils {

    int getWordSize(char*);

    bool isInt(char*);
    
    bool isLong(char*);

    bool isDouble(char*);

    int getStringSize(char*);

    char* copyString(char*, int);

}  

#endif