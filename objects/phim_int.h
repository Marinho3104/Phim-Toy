#pragma once

#ifndef PHIM_INT_H
#define PHIM_INT_H

#include "./phim_object.h"

namespace objects {

    struct Phim_Int : public Phim_Object<4> {

        ~Phim_Int() = default;

        Phim_Int();

        Phim_Int(char*);


    };

}

#endif