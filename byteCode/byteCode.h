#pragma once

#ifndef BYTE_CODE_H
#define BYTE_CODE_H

namespace utils { template <typename> struct LinkedList; }

namespace byte_code {

    struct Byte_Code {

        unsigned char code;
        int argument;

        ~Byte_Code() = default;

        Byte_Code(unsigned char, int);

    };

    struct Block_Code {

        utils::LinkedList <Byte_Code>* byteCode;
        

    };

    struct Program_Out {

        // utils::LinkedList <Byte_Code>* 

    };

}

#endif