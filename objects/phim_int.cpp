#include "./phim_int.h"

#include <iostream>


void objects::Phim_Int::fromCharToInt(char* __i, int* __r) {

    *__r = 0;

    for (; *__i != '\0';)

        *__r = *__r * 10 + *(__i++) - '0';

}
