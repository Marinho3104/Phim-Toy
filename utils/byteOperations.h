#pragma once

#ifndef BYTEOPERATIONS_H
#define BYTEOPERATIONS_H

namespace utils {

    /**/
    bool getBit(unsigned char, int);
    /**/
    void getByteRepresentation(unsigned char, char*);
    /**/
    bool setBit(unsigned char*, int, bool);
    /**/
    unsigned char addBytes(unsigned char, unsigned char, bool&);
    /**/
    unsigned char subBytes(unsigned char, unsigned char, bool&);

}

#endif