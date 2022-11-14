#pragma once

#ifndef PHIM_INT_H
#define PHIM_INT_H

#include "./phim_object.h"

namespace objects {

    struct Phim_Int : public Phim_Object {

        unsigned char data[4]; // Represents bytes information be aware that is backwards meaning -> data[0] is the biggest bit

        ~Phim_Int() = default;

        Phim_Int();

        unsigned char* getData();

        int getDataLen();


    };

    Phim_Int* PhimIntFromCString(char*);

}

#endif