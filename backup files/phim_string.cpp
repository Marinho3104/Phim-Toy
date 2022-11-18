#include "./phim_string.h"

#include "./../utils/byteOperations.h"

#include <iostream>
#include <string.h>

objects::Phim_String::Phim_String() : data(NULL) {}

objects::Phim_String::Phim_String(char* _data) {

    int _ = strlen(_data);

    data = new unsigned char[_ + 1];

    for (int i = 0; i < _; i++)

        data[i] = (int) (unsigned char) _data[i];

    data[_] = '\0';

}

unsigned char* objects::Phim_String::getData() { return data; }

int objects::Phim_String::getDataLen() { return strlen( (const char*) data); }

void objects::Phim_String::toString() {

    size_t _ = 0;

    while(data[_]) {

        std::cout << data[_++];

    }

    std::cout << std::endl;

}


objects::Phim_String* objects::Phim_String::operator&(objects::Phim_Object* _) {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String)); 
    new(_rtr) objects::Phim_String(); _rtr->clean();

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        _rtr->setByte(
            _crrntByte,
            operator[](_crrntByte) & (*_)[_crrntByte]
        );

    return _rtr;

}

objects::Phim_String* objects::Phim_String::operator|(objects::Phim_Object* _) {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String)); 
    new(_rtr) objects::Phim_String(); _rtr->clean();

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        _rtr->setByte(
            _crrntByte,
            operator[](_crrntByte) | (*_)[_crrntByte]
        );

    return _rtr;

}

objects::Phim_String* objects::Phim_String::operator^(objects::Phim_Object* _) {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String)); 
    new(_rtr) objects::Phim_String(); _rtr->clean();

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        _rtr->setByte(
            _crrntByte,
            operator[](_crrntByte) ^ (*_)[_crrntByte]
        );

    return _rtr;

}

objects::Phim_String* objects::Phim_String::operator~() {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String)); 
    new(_rtr) objects::Phim_String(); _rtr->clean();

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        _rtr->setByte(
            _crrntByte,
            ~operator[](_crrntByte)
        );

    return _rtr;

}

objects::Phim_String* objects::Phim_String::operator+(objects::Phim_Object* _) {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String)); 
    new(_rtr) objects::Phim_String(); _rtr->clean();
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

objects::Phim_String* objects::Phim_String::operator-(objects::Phim_Object* _) {
    
    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String)); 
    new(_rtr) objects::Phim_String(); _rtr->clean();
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

objects::Phim_String* objects::Phim_String::operator*(objects::Phim_Object* _) {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String));
    objects::Phim_String* _mul1 = (objects::Phim_String*) malloc(sizeof(objects::Phim_String));
    objects::Phim_String* _mul2 = (objects::Phim_String*) malloc(sizeof(objects::Phim_String));
    new (_rtr) objects::Phim_String();
    new (_mul1) objects::Phim_String();
    new (_mul2) objects::Phim_String();

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

objects::Phim_String* objects::Phim_String::operator/(objects::Phim_Object* _) {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String));
    objects::Phim_String* _temp = (objects::Phim_String*) malloc(sizeof(objects::Phim_String));
    new (_rtr) objects::Phim_String();
    new (_temp) objects::Phim_String();

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

objects::Phim_String* objects::Phim_String::operator%(objects::Phim_Object* _) {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String));
    objects::Phim_String* _temp = (objects::Phim_String*) malloc(sizeof(objects::Phim_String));
    new (_rtr) objects::Phim_String();
    new (_temp) objects::Phim_String();

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

objects::Phim_String* objects::Phim_String::operator<<(int _) {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String)); new(_rtr) objects::Phim_String();
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

objects::Phim_String* objects::Phim_String::operator>>(int _) {

    objects::Phim_String* _rtr = (objects::Phim_String*) malloc(sizeof(objects::Phim_String)); new(_rtr) objects::Phim_String();
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
