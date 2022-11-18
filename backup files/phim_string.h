#pragma once

#ifndef PHIM_STRING_H
#define PHIM_STRING_H

#include "./phim_object.h"


namespace objects {

    // Can be done with Phim_Char too

    struct Phim_String : public Phim_Object {

        unsigned char* data;

        ~Phim_String() = default;

        Phim_String();

        Phim_String(char*);

        unsigned char* getData();

        int getDataLen();

        void toString();

        Phim_String* operator&(Phim_Object*);
        Phim_String* operator|(Phim_Object*);
        Phim_String* operator^(Phim_Object*);
        Phim_String* operator~();

        Phim_String* operator+(Phim_Object*);
        Phim_String* operator-(Phim_Object*);

        Phim_String* operator*(Phim_Object*);
        Phim_String* operator/(Phim_Object*);
        Phim_String* operator%(Phim_Object*);

        Phim_String* operator<<(int);
        Phim_String* operator>>(int);

    };

}

#endif