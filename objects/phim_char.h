#pragma once


#ifndef PHIM_CHAR_H
#define PHIM_CHAR_H

#include "./phim_object.h"

namespace objects {

    struct Phim_Char : public Phim_Object {

        ~Phim_Char() = default;
        
        Phim_Char();

        Phim_Char(char);

    };

}

#endif