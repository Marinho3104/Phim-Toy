#pragma once

#ifndef PHIM_STRING_H
#define PHIM_STRING_H

#include "./phim_object.h"
#include "./phim_char.h"

namespace objects {

    struct Phim_String : public Phim_Object {

        Phim_Char* str;

        ~Phim_String() = default;

        Phim_String();

        Phim_String(char*);

        void toString();

    };

}

#endif