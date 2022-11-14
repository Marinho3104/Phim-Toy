#pragma once

#ifndef PHIM_STRING_H
#define PHIM_STRING_H

#include "./phim_object.h"

namespace objects {

    template <int strSize>
    struct Phim_String : public Phim_Object<strSize> {

        ~Phim_String() = default;

        Phim_String();

        Phim_String(char*);

        char* toString();

    };

}

#include "./phim_string.cpp"

#endif