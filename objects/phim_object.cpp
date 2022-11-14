#include "./../utils/byteOperations.h"

#include <iostream>

template <int bytesCount>
objects::Phim_Object<bytesCount>::~Phim_Object() {}

template <int bytesCount>
objects::Phim_Object<bytesCount>::Phim_Object() { clean(); }

template <int bytesCount>
void objects::Phim_Object<bytesCount>::clean() { for (int _ = 0; _ < bytesCount; _++) data[_] = 0; }

template <int bytesCount>
void objects::Phim_Object<bytesCount>::copyBytes(unsigned char* _cpy, int _s, int _inPos) { 
    while(_s && _inPos < bytesCount) data[_inPos++] = _cpy[bytesCount - _s--]; }

template <int bytesCount>
unsigned char& objects::Phim_Object<bytesCount>::operator[](int _) { if (_ >= bytesCount; _ < 0) _ = 0; return data[_]; }

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::getBit(int _) { if (_ >= bytesCount * 8 || _ < 0) _ = 0; return utils::getBit(operator[](_ / 8), _ - _ / 8 * 8); }

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::setByte(int _, unsigned char _v) { if (_ >= bytesCount || _ < 0) return false; operator[](_) = _v; }

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::setBit(int _, bool _v) {

    if (_ >= bytesCount * 8 || _ < 0) return false;

    return utils::setBit(
        data + _ / 8,
        _ - _ / 8 * 8,
        _v
    );

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::bitsRepresentation() {

    char _rtr[bytesCount * 9];
    char* _t = _rtr;

    for (int _ = 0; _ < bytesCount; _++) {

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

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::isOdd() { return getBit(bytesCount * 8 - 1); }

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::isZero() {
    for (int _ = 0; _ < bytesCount; _++)
        if (operator[](_)) return false;
    return true;
}


template <int bytesCount>
bool objects::Phim_Object<bytesCount>::operator<(objects::Phim_Object<bytesCount>& _) {

    unsigned char _val1, _val2;
    bool _rtr = 0;

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++) {

        _val1 = operator[](_crrntByte);
        _val2 = _[_crrntByte];

        if (_val1 == _val2) continue;

        if (_val1 < _val2) _rtr = 1;

        break; 

    }

    return _rtr;

}

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::operator>(objects::Phim_Object<bytesCount>& _) {

    unsigned char _val1, _val2;
    bool _rtr = 0;

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++) {

        _val1 = operator[](_crrntByte);
        _val2 = _[_crrntByte];

        if (_val1 == _val2) continue;

        if (_val1 > _val2) _rtr = 1;

        break; 

    }

    return _rtr;

}

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::operator<=(objects::Phim_Object<bytesCount>& _) {

    unsigned char _val1, _val2;
    bool _rtr = 1;

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++) {

        _val1 = operator[](_crrntByte);
        _val2 = _[_crrntByte];

        if (_val1 == _val2) continue;

        if (_val1 > _val2) _rtr = 0;

        break; 

    }

    return _rtr;

}

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::operator>=(objects::Phim_Object<bytesCount>& _) {

    unsigned char _val1, _val2;
    bool _rtr = 1;

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++) {

        _val1 = operator[](_crrntByte);
        _val2 = _[_crrntByte];

        if (_val1 == _val2) continue;

        if (_val1 < _val2) _rtr = 0;

        break; 

    }

    return _rtr;

}

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::operator==(objects::Phim_Object<bytesCount>& _) {

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++) if (operator[](_crrntByte) != _[_crrntByte]) return false;

    return true;

}

template <int bytesCount>
bool objects::Phim_Object<bytesCount>::operator!=(objects::Phim_Object<bytesCount>& _) { return !operator==(_); }  

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator&(objects::Phim_Object<bytesCount>& _) {

    objects::Phim_Object<bytesCount> _rtr; _rtr.clean();

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++)

        _rtr.setByte(
            _crrntByte,
            operator[](_crrntByte) & _[_crrntByte]
        );

    return _rtr;

}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator|(objects::Phim_Object<bytesCount>& _) {

    objects::Phim_Object<bytesCount> _rtr; _rtr.clean();

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++)

        _rtr.setByte(
            _crrntByte,
            operator[](_crrntByte) | _[_crrntByte]
        );

    return _rtr;

}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator^(objects::Phim_Object<bytesCount>& _) {

    objects::Phim_Object<bytesCount> _rtr; _rtr.clean();

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++)

        _rtr.setByte(
            _crrntByte,
            operator[](_crrntByte) ^ _[_crrntByte]
        );

    return _rtr;

}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator~() {

    objects::Phim_Object<bytesCount> _rtr; _rtr.clean();

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++)

        _rtr.setByte(
            _crrntByte,
            ~operator[](_crrntByte)
        );

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator&=(objects::Phim_Object<bytesCount>& _) {

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++)

        setByte(
            _crrntByte,
            operator[](_crrntByte) & _[_crrntByte]
        );

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator|=(objects::Phim_Object<bytesCount>& _) {

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++)

        setByte(
            _crrntByte,
            operator[](_crrntByte) | _[_crrntByte]
        );

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator^=(objects::Phim_Object<bytesCount>& _) {

    for (int _crrntByte = 0; _crrntByte < bytesCount; _crrntByte++)

        setByte(
            _crrntByte,
            operator[](_crrntByte) ^ _[_crrntByte]
        );

}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator+(objects::Phim_Object<bytesCount>& _) {

    objects::Phim_Object<bytesCount> _rtr;
    bool _over = false;
    _rtr.clean();

    for (int _crrntByte = bytesCount - 1; _crrntByte >= 0; _crrntByte--)

        _rtr.setByte(
            _crrntByte,
            utils::addBytes(
                operator[](_crrntByte),
                _[_crrntByte],
                _over
            )
        );

    return _rtr;

}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator-(objects::Phim_Object<bytesCount>& _) {
    
    objects::Phim_Object<bytesCount> _rtr;
    bool _over = false;
    _rtr.clean();

    for (int _crrntByte = bytesCount - 1; _crrntByte >= 0; _crrntByte--)

        _rtr.setByte(
            _crrntByte,
            utils::subBytes(
                operator[](_crrntByte),
                _[_crrntByte],
                _over
            )
        );

    return _rtr;

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator+=(objects::Phim_Object<bytesCount>& _) {

    bool _over = false;

    for (int _crrntByte = bytesCount - 1; _crrntByte >= 0; _crrntByte--)

        setByte(
            _crrntByte,
            utils::addBytes(
                operator[](_crrntByte),
                _[_crrntByte],
                _over
            )
        );
    
}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator-=(objects::Phim_Object<bytesCount>& _) {

    bool _over = false;

    for (int _crrntByte = bytesCount - 1; _crrntByte >= 0; _crrntByte--)

        setByte(
            _crrntByte,
            utils::subBytes(
                operator[](_crrntByte),
                _[_crrntByte],
                _over
            )
        );
}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator*(objects::Phim_Object<bytesCount>& _) {

    objects::Phim_Object <bytesCount> _rtr, _mul1, _mul2;
    
    _rtr.clean();
    _mul1.copyBytes(data, bytesCount, 0);
    _mul2.copyBytes(_.data, bytesCount, 0);

    while(!_mul2.isZero()) {

        if (_mul2.isOdd()) _rtr += _mul1;

        _mul2 >>= 1;
        _mul1 <<= 1;

    }

    return _rtr;

}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator/(objects::Phim_Object<bytesCount>& _) {

    objects::Phim_Object<bytesCount> _rtr, _temp;
    _rtr.clean(); _temp.clean();
    int _crrntBit = 0;

    while(1) {

        while(1) {

            if (_crrntBit >= bytesCount * 8) goto rtrn;

            _temp <<= 1;

            _temp.setBit(
                bytesCount * 8 - 1,
                getBit(_crrntBit++)
            );

            if (_temp >= _) break;

            _rtr <<= 1;

        }

        _temp -= _;

        _rtr <<= 1;

        _rtr.setBit(bytesCount * 8 - 1, 1);

    }

rtrn:
    return _rtr;

}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator%(objects::Phim_Object<bytesCount>& _) {

    objects::Phim_Object<bytesCount> _rtr, _temp;
    _rtr.clean(); _temp.clean();
    int _crrntBit = 0;

    while(1) {

        while(1) {

            if (_crrntBit >= bytesCount * 8) goto rtrn;

            _temp <<= 1;

            _temp.setBit(
                bytesCount * 8 - 1,
                getBit(_crrntBit++)
            );

            if (_temp >= _) break;

            _rtr <<= 1;

        }

        _temp -= _;

        _rtr <<= 1;

        _rtr.setBit(bytesCount * 8 - 1, 1);

    }

rtrn:
    return _temp;

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator*=(objects::Phim_Object<bytesCount>& _) {

    objects::Phim_Object <bytesCount> _mul1, _mul2;
    
    _mul1.copyBytes(data, bytesCount, 0);
    _mul2.copyBytes(_.data, bytesCount, 0);

    while(!_mul2.isZero()) {

        if (_mul2.isOdd()) *this += _mul1;

        _mul2 >>= 1;
        _mul1 <<= 1;

    }

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator/=(objects::Phim_Object<bytesCount>& _) {
    objects::Phim_Object<bytesCount> _rtr, _temp;
    _rtr.clean(); _temp.clean();
    int _crrntBit = 0;

    while(1) {

        while(1) {

            if (_crrntBit >= bytesCount * 8) { copyBytes(_rtr.data, bytesCount, 0); return; }

            _temp <<= 1;

            _temp.setBit(
                bytesCount * 8 - 1,
                getBit(_crrntBit++)
            );

            if (_temp >= _) break;

            _rtr <<= 1;

        }

        _temp -= _;

        _rtr <<= 1;

        _rtr.setBit(bytesCount * 8 - 1, 1);

    }

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator%=(objects::Phim_Object<bytesCount>& _) {

    objects::Phim_Object<bytesCount> _rtr, _temp;
    _rtr.clean(); _temp.clean();
    int _crrntBit = 0;

    while(1) {

        while(1) {

            if (_crrntBit >= bytesCount * 8) { copyBytes(_temp.data, bytesCount, 0); return; }

            _temp <<= 1;

            _temp.setBit(
                bytesCount * 8 - 1,
                getBit(_crrntBit++)
            );

            if (_temp >= _) break;

            _rtr <<= 1;

        }

        _temp -= _;

        _rtr <<= 1;

        _rtr.setBit(bytesCount * 8 - 1, 1);

    }

}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator<<(int _) {

    objects::Phim_Object <bytesCount> _rtr;
    _rtr.copyBytes(data, bytesCount, 0);

    for (int _crrntShift = 0; _crrntShift < _; _crrntShift++) {

        for (int _crrntBit = 0; _crrntBit < bytesCount * 8 - 1; _crrntBit++) 

            _rtr.setBit(
                _crrntBit,
                _rtr.getBit(_crrntBit + 1)
            );

        _rtr.setBit(
            bytesCount * 8 - 1,
            0
        );
    
    }

    return _rtr;

}

template <int bytesCount>
objects::Phim_Object<bytesCount> objects::Phim_Object<bytesCount>::operator>>(int _) {

    objects::Phim_Object <bytesCount> _rtr;
    _rtr.copyBytes(data, bytesCount, 0);

    for (int _crrntShift = 0; _crrntShift < _; _crrntShift++) {

        for (int _crrntBit = bytesCount * 8 - 1; _crrntBit >= 0; _crrntBit--) 

            _rtr.setBit(
                _crrntBit,
                _rtr.getBit(_crrntBit - 1)
            );

        _rtr.setBit(
            0,
            0
        );
    }

    return _rtr;

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator<<=(int _) {

    for (int _crrntShift = 0; _crrntShift < _; _crrntShift++) {

        for (int _crrntBit = 0; _crrntBit < bytesCount * 8 - 1; _crrntBit++) 

            setBit(
                _crrntBit,
                getBit(_crrntBit + 1)
            );

        setBit(
            bytesCount * 8 - 1,
            0
        );
    
    }

}

template <int bytesCount>
void objects::Phim_Object<bytesCount>::operator>>=(int _) {

    for (int _crrntShift = 0; _crrntShift < _; _crrntShift++) {

        for (int _crrntBit = bytesCount * 8 - 1; _crrntBit >= 0; _crrntBit--) 

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



