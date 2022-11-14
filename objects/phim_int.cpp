#include "./phim_int.h"

#include <iostream>

objects::Phim_Int::Phim_Int() : Phim_Object() {}

objects::Phim_Int::Phim_Int(char* _) {

    int _t = 0;

    for (; *_ != '\0';)

        _t = _t * 10 + *(_++) - '0';

    for (int _c = 3; _c >= 0; _c--) {

        setByte(_c, _t);

        _t >>= 8;

    }

}



