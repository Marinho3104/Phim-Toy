#include "./phim_char.h"

#include "./../utils/byteOperations.h"

#include <iostream>

objects::Phim_Char::Phim_Char() : Phim_Object() {}

objects::Phim_Char::Phim_Char(char _) : Phim_Object() { data[0] = (int) (unsigned char) _; }

unsigned char* objects::Phim_Char::getData() { return data; }

int objects::Phim_Char::getDataLen() { return 1; }

objects::Phim_Char* objects::Phim_Char::operator&(objects::Phim_Object* _) {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char)); 
    new(_rtr) objects::Phim_Char(); _rtr->clean();

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        _rtr->setByte(
            _crrntByte,
            operator[](_crrntByte) & (*_)[_crrntByte]
        );

    return _rtr;

}

objects::Phim_Char* objects::Phim_Char::operator|(objects::Phim_Object* _) {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char)); 
    new(_rtr) objects::Phim_Char(); _rtr->clean();

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        _rtr->setByte(
            _crrntByte,
            operator[](_crrntByte) | (*_)[_crrntByte]
        );

    return _rtr;

}

objects::Phim_Char* objects::Phim_Char::operator^(objects::Phim_Object* _) {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char)); 
    new(_rtr) objects::Phim_Char(); _rtr->clean();

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        _rtr->setByte(
            _crrntByte,
            operator[](_crrntByte) ^ (*_)[_crrntByte]
        );

    return _rtr;

}

objects::Phim_Char* objects::Phim_Char::operator~() {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char)); 
    new(_rtr) objects::Phim_Char(); _rtr->clean();

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        _rtr->setByte(
            _crrntByte,
            ~operator[](_crrntByte)
        );

    return _rtr;

}

objects::Phim_Char* objects::Phim_Char::operator+(objects::Phim_Object* _) {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char)); 
    new(_rtr) objects::Phim_Char(); _rtr->clean();
    bool _over = false;

    for (int _crrntByte = getDataLen() - 1; _crrntByte >= 0; _crrntByte--)

        _rtr->setByte(
            _crrntByte,
            utils::addBytes(
                operator[](_crrntByte),
                (*_)[_crrntByte],
                _over
            )
        );

    return _rtr;

}

objects::Phim_Char* objects::Phim_Char::operator-(objects::Phim_Object* _) {
    
    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char)); 
    new(_rtr) objects::Phim_Char(); _rtr->clean();
    bool _over = false;

    for (int _crrntByte = getDataLen() - 1; _crrntByte >= 0; _crrntByte--)

        _rtr->setByte(
            _crrntByte,
            utils::subBytes(
                operator[](_crrntByte),
                (*_)[_crrntByte],
                _over
            )
        );

    return _rtr;

}

objects::Phim_Char* objects::Phim_Char::operator*(objects::Phim_Object* _) {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char));
    objects::Phim_Char* _mul1 = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char));
    objects::Phim_Char* _mul2 = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char));
    new (_rtr) objects::Phim_Char();
    new (_mul1) objects::Phim_Char();
    new (_mul2) objects::Phim_Char();

    _rtr->clean();
    _mul1->copyBytes(getData(), getDataLen(), 0);
    _mul2->copyBytes((*_).getData(), getDataLen(), 0);

    while(!_mul2->isZero()) {

        if (_mul2->isOdd()) *_rtr += _mul1;

        *_mul2 >>= 1;
        *_mul1 <<= 1;

    }

    return _rtr;

}

objects::Phim_Char* objects::Phim_Char::operator/(objects::Phim_Object* _) {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char));
    objects::Phim_Char* _temp = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char));
    new (_rtr) objects::Phim_Char();
    new (_temp) objects::Phim_Char();

    _rtr->clean(); _temp->clean();
    int _crrntBit = 0;

    while(1) {

        while(1) {

            if (_crrntBit >= getDataLen() * 8) goto rtrn;

            *_temp <<= 1;

            _temp->setBit(
                getDataLen() * 8 - 1,
                getBit(_crrntBit++)
            );

            if (*_temp >= _) break;

            *_rtr <<= 1;

        }

        *_temp -= _;

        *_rtr <<= 1;

        _rtr->setBit(getDataLen() * 8 - 1, 1);

    }

rtrn:
    return _rtr;

}

objects::Phim_Char* objects::Phim_Char::operator%(objects::Phim_Object* _) {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char));
    objects::Phim_Char* _temp = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char));
    new (_rtr) objects::Phim_Char();
    new (_temp) objects::Phim_Char();

    _rtr->clean(); _temp->clean();
    int _crrntBit = 0;

    while(1) {

        while(1) {

            if (_crrntBit >= getDataLen() * 8) goto rtrn;

            *_temp <<= 1;

            _temp->setBit(
                getDataLen() * 8 - 1,
                getBit(_crrntBit++)
            );

            if (*_temp >= _) break;

            *_rtr <<= 1;

        }

        *_temp -= _;

        *_rtr <<= 1;

        _rtr->setBit(getDataLen() * 8 - 1, 1);

    }

rtrn:
    return _temp;

}

objects::Phim_Char* objects::Phim_Char::operator<<(int _) {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char)); new(_rtr) objects::Phim_Char();
    _rtr->copyBytes(getData(), getDataLen(), 0);

    for (int _crrntShift = 0; _crrntShift < _; _crrntShift++) {

        for (int _crrntBit = 0; _crrntBit < getDataLen() * 8 - 1; _crrntBit++) 

            _rtr->setBit(
                _crrntBit,
                _rtr->getBit(_crrntBit + 1)
            );

        _rtr->setBit(
            getDataLen() * 8 - 1,
            0
        );
    
    }

    return _rtr;

}

objects::Phim_Char* objects::Phim_Char::operator>>(int _) {

    objects::Phim_Char* _rtr = (objects::Phim_Char*) malloc(sizeof(objects::Phim_Char)); new(_rtr) objects::Phim_Char();
    _rtr->copyBytes(getData(), getDataLen(), 0);

    for (int _crrntShift = 0; _crrntShift < _; _crrntShift++) {

        for (int _crrntBit = getDataLen() * 8 - 1; _crrntBit >= 0; _crrntBit--) 

            _rtr->setBit(
                _crrntBit,
                _rtr->getBit(_crrntBit - 1)
            );

        _rtr->setBit(
            0,
            0
        );
    }

    return _rtr;

}


