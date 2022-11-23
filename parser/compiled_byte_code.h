#pragma once

#ifndef COMPILED_BYTE_CODE_H
#define COMPILER_BYTE_CODE_H

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Type_Information;

    struct Byte_Code_Block {

        utils::LinkedList <byte_code::Byte_Code*>* byte_code_block;

        Byte_Code_Block(utils::LinkedList <byte_code::Byte_Code*>*);

    };

    struct Compiled_Byte_Code {

        utils::LinkedList <Byte_Code_Block*>* byte_code_blocks;
        utils::LinkedList <Type_Information*>* types;
        utils::LinkedList <char*>* implicit_values; 

        Compiled_Byte_Code(utils::LinkedList <Byte_Code_Block*>*, utils::LinkedList <Type_Information*>*, utils::LinkedList <char*>*);

        void print();

    };

}

#endif