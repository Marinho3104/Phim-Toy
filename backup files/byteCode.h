#pragma once

#ifndef BYTE_CODE_H
#define BYTE_CODE_H

namespace byte_code {

    struct Byte_Code {

        unsigned char code;
        int argument;

        ~Byte_Code() = default;

        Byte_Code(unsigned char, int);

    };

}

#endif