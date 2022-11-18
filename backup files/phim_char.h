#pragma once


#ifndef PHIM_CHAR_H
#define PHIM_CHAR_H

#include "./phim_object.h"

namespace objects {

    struct Phim_Char : public Phim_Object {

        unsigned char data[1];

        ~Phim_Char() = default;
        
        Phim_Char();

        Phim_Char(char);

        unsigned char* getData();

        int getDataLen();

        Phim_Char* operator&(Phim_Object*);
        Phim_Char* operator|(Phim_Object*);
        Phim_Char* operator^(Phim_Object*);
        Phim_Char* operator~();

        Phim_Char* operator+(Phim_Object*);
        Phim_Char* operator-(Phim_Object*);

        Phim_Char* operator*(Phim_Object*);
        Phim_Char* operator/(Phim_Object*);
        Phim_Char* operator%(Phim_Object*);

        Phim_Char* operator<<(int);
        Phim_Char* operator>>(int);

    };

}

#endif