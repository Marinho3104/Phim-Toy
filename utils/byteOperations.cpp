#include "./byteOperations.h"

#include <iostream>

bool utils::getBit(unsigned char _byte, int _index) { return _byte << _index & 0x80; }

void utils::getByteRepresentation(unsigned char _byte, char* _rtr) {
    for (int _ = 0; _ < 8; _++) *(_rtr++) = (_byte << _) & 0x80 ? '1' : '0';
}

bool utils::setBit(unsigned char* _b, int _i, bool _v) {

    if (_i > 7 || _i < 0) return false;

    *_b = (*_b & ~(1U << 7 - _i)) | (_v << 7 -_i);

    return true;

}

unsigned char utils::addBytes(unsigned char _b1, unsigned char _b2, bool& _) {
    unsigned char _rtr = _b1 + _b2 + _;
    _ = (_b1 + _b2 + _ > 255);
    return _rtr;
}

unsigned char utils::subBytes(unsigned char _b1, unsigned char _b2, bool& _) {
    unsigned char _rtr = _b1 - _b2 - _;
    _ = (_b2 + _ > _b1);
    return _rtr;
}








