#include "./phim_int.h"

#include "./../utils/byteOperations.h"

#include <iostream>

objects::Phim_Int::Phim_Int() {}

unsigned char* objects::Phim_Int::getData() { return data; }

int objects::Phim_Int::getDataLen() { return 4; }

objects::Phim_Int* objects::PhimIntFromCString(char* _) {

    objects::Phim_Int* _rtr = new objects::Phim_Int();
    _rtr->clean();

    int _t = 0;

    for (; *_ != '\0';)

        _t = _t * 10 + *(_++) - '0';

    for (int _c = 3; _c >= 0; _c--) {

        _rtr->setByte(_c, _t);

        _t >>= 8;

    }

    return _rtr;

}



