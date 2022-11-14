#include "./phim_string.h"

#include <iostream>


template <int strSize>
objects::Phim_String<strSize>::Phim_String() {}

template <int strSize>
objects::Phim_String<strSize>::Phim_String(char* _) {

    for (int __ = 0; __ < strSize; __++)

        this->setByte(__, (unsigned char) _[__]);

}   

template <int strSize>
char* objects::Phim_String<strSize>::toString() {

    char* _rtr = new char[strSize + 1];

    for (int _ = 0; _ < strSize; _++)

        _rtr[_] = this->operator[](_);

    _rtr[strSize] = '\0';

    return _rtr;

}



