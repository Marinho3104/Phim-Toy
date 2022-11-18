#include "./../utils/byteOperations.h"

#include "./phim_object.h"

#include <iostream>


objects::Phim_Object::~Phim_Object() {}

objects::Phim_Object::Phim_Object() { } // clean(); }

void objects::Phim_Object::clean() { for (int _ = 0; _ < getDataLen(); _++) getData()[_] = 0; }

void objects::Phim_Object::copyBytes(unsigned char* _cpy, int _s, int _inPos) { 
    while(_s && _inPos < getDataLen()) getData()[_inPos++] = _cpy[getDataLen() - _s--]; }

unsigned char& objects::Phim_Object::operator[](int _) { if (_ >= getDataLen() || _ < 0) _ = 0; return getData()[_]; }

bool objects::Phim_Object::getBit(int _) { if (_ >= getDataLen() * 8 || _ < 0) _ = 0; return utils::getBit(operator[](_ / 8), _ - _ / 8 * 8); }

bool objects::Phim_Object::setByte(int _, unsigned char _v) { if (_ >= getDataLen() || _ < 0) return false; operator[](_) = _v; }

bool objects::Phim_Object::setBit(int _, bool _v) {

    if (_ >= getDataLen() * 8 || _ < 0) return false;

    return utils::setBit(
        getData() + _ / 8,
        _ - _ / 8 * 8,
        _v
    );

}

void objects::Phim_Object::bitsRepresentation() {

    char _rtr[getDataLen() * 9];
    char* _t = _rtr;

    for (int _ = 0; _ < getDataLen(); _++) {

        utils::getByteRepresentation(
            operator[](_),
            _t
        );

        _t+=8;

        *(_t++) = ' ';

    }

    *(--_t) = 0;

    std::cout << _rtr << std::endl;

}

bool objects::Phim_Object::isOdd() { return getBit(getDataLen() * 8 - 1); }

bool objects::Phim_Object::isZero() {
    for (int _ = 0; _ < getDataLen(); _++)
        if (operator[](_)) return false;
    return true;
}


bool objects::Phim_Object::operator<(objects::Phim_Object* _) {

    unsigned char _val1, _val2;
    bool _rtr = 0;

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++) {

        _val1 = operator[](_crrntByte);
        _val2 = (*_)[_crrntByte];

        if (_val1 == _val2) continue;

        if (_val1 < _val2) _rtr = 1;

        break; 

    }

    return _rtr;

}

bool objects::Phim_Object::operator>(objects::Phim_Object* _) {

    unsigned char _val1, _val2;
    bool _rtr = 0;

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++) {

        _val1 = operator[](_crrntByte);
        _val2 = (*_)[_crrntByte];

        if (_val1 == _val2) continue;

        if (_val1 > _val2) _rtr = 1;

        break; 

    }

    return _rtr;

}

bool objects::Phim_Object::operator<=(objects::Phim_Object* _) {

    unsigned char _val1, _val2;
    bool _rtr = 1;

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++) {

        _val1 = operator[](_crrntByte);
        _val2 = (*_)[_crrntByte];

        if (_val1 == _val2) continue;

        if (_val1 > _val2) _rtr = 0;

        break; 

    }

    return _rtr;

}

bool objects::Phim_Object::operator>=(objects::Phim_Object* _) {

    unsigned char _val1, _val2;
    bool _rtr = 1;

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++) {

        _val1 = operator[](_crrntByte);
        _val2 = (*_)[_crrntByte];

        if (_val1 == _val2) continue;

        if (_val1 < _val2) _rtr = 0;

        break; 

    }

    return _rtr;

}

bool objects::Phim_Object::operator==(objects::Phim_Object* _) {

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++) if (operator[](_crrntByte) != (*_)[_crrntByte]) return false;

    return true;

}

bool objects::Phim_Object::operator!=(objects::Phim_Object* _) { return !operator==(_); }  

void objects::Phim_Object::operator&=(objects::Phim_Object* _) {

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        setByte(
            _crrntByte,
            operator[](_crrntByte) & (*_)[_crrntByte]
        );

}

void objects::Phim_Object::operator|=(objects::Phim_Object* _) {

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        setByte(
            _crrntByte,
            operator[](_crrntByte) | (*_)[_crrntByte]
        );

}

void objects::Phim_Object::operator^=(objects::Phim_Object* _) {

    for (int _crrntByte = 0; _crrntByte < getDataLen(); _crrntByte++)

        setByte(
            _crrntByte,
            operator[](_crrntByte) ^ (*_)[_crrntByte]
        );

}

void objects::Phim_Object::operator+=(objects::Phim_Object* _) {

    bool _over = false;

    for (int _crrntByte = getDataLen() - 1; _crrntByte >= 0; _crrntByte--)

        setByte(
            _crrntByte,
            utils::addBytes(
                operator[](_crrntByte),
                (*_)[_crrntByte],
                _over
            )
        );
    
}

void objects::Phim_Object::operator-=(objects::Phim_Object* _) {

    bool _over = false;

    for (int _crrntByte = getDataLen() - 1; _crrntByte >= 0; _crrntByte--)

        setByte(
            _crrntByte,
            utils::subBytes(
                operator[](_crrntByte),
                (*_)[_crrntByte],
                _over
            )
        );
}

void objects::Phim_Object::operator*=(objects::Phim_Object* _) {

    objects::Phim_Object* _mul1, *_mul2;
    
    _mul1->copyBytes(getData(), getDataLen(), 0);
    _mul2->copyBytes(_->getData(), getDataLen(), 0);

    while(!_mul2->isZero()) {

        if (_mul2->isOdd()) *this += _mul1;

        *_mul2 >>= 1;
        *_mul1 <<= 1;

    }

}

void objects::Phim_Object::operator/=(objects::Phim_Object* _) {
    objects::Phim_Object* _rtr, *_temp;
    _rtr->clean(); _temp->clean();
    int _crrntBit = 0;

    while(1) {

        while(1) {

            if (_crrntBit >= getDataLen() * 8) { copyBytes(_rtr->getData(), getDataLen(), 0); return; }

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

}

void objects::Phim_Object::operator%=(objects::Phim_Object* _) {

    objects::Phim_Object* _rtr, *_temp;
    _rtr->clean(); _temp->clean();
    int _crrntBit = 0;

    while(1) {

        while(1) {

            if (_crrntBit >= getDataLen() * 8) { copyBytes(_temp->getData(), getDataLen(), 0); return; }

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

}

void objects::Phim_Object::operator<<=(int _) {

    for (int _crrntShift = 0; _crrntShift < _; _crrntShift++) {

        for (int _crrntBit = 0; _crrntBit < getDataLen() * 8 - 1; _crrntBit++) 

            setBit(
                _crrntBit,
                getBit(_crrntBit + 1)
            );

        setBit(
            getDataLen() * 8 - 1,
            0
        );
    
    }

}

void objects::Phim_Object::operator>>=(int _) {

    for (int _crrntShift = 0; _crrntShift < _; _crrntShift++) {

        for (int _crrntBit = getDataLen() * 8 - 1; _crrntBit >= 0; _crrntBit--) 

            setBit(
                _crrntBit,
                getBit(_crrntBit - 1)
            );

        setBit(
            0,
            0
        );
    }

}

