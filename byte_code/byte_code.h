#pragma once

#ifndef BYTE_CODE_H
#define BYTE_CODE_H

namespace utils { template <typename> struct Linked_List; }

namespace byte_code {

    struct Byte_Code {

        char code;
        int argument;

        ~Byte_Code(); Byte_Code(char, int);

    };

    struct Byte_Code_Block {

        utils::Linked_List <Byte_Code*>* block;

        ~Byte_Code_Block(); Byte_Code_Block(utils::Linked_List <Byte_Code*>*);

    };

    struct Compiled_Byte_Code {

        utils::Linked_List <Byte_Code_Block*>* blocks;
        utils::Linked_List <char*>* implicit_values;

        ~Compiled_Byte_Code(); Compiled_Byte_Code(utils::Linked_List <Byte_Code_Block*>*, utils::Linked_List <char*>*);

        void print();

    };

}

#endif