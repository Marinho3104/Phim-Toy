#pragma once

#ifndef BYTE_CODE_CONVERTER_H
#define BYTE_CODE_CONVERTER_H

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Byte_Code_Converter_Control;
    struct Ast_Node_Block;
    struct Storage;

    struct Byte_Code_Converter_Block {

        int pos; 
        utils::LinkedList <byte_code::Byte_Code*>* byteCode; // Block byteCode

        Byte_Code_Converter_Block(int, utils::LinkedList <byte_code::Byte_Code*>*);
        /**/
        static Byte_Code_Converter_Block* generate(Byte_Code_Converter_Control*, Ast_Node_Block*, int);

    };

    struct Byte_Code_Converter_Control_Exception {
        char* description; // Description of exception
        Byte_Code_Converter_Control_Exception(char*);
    };

    struct Byte_Code_Converter_Control {

        utils::LinkedList <Byte_Code_Converter_Block*>* byteCodeBlocks;
        Storage* storage;
        Ast_Node_Block* ast;

        Byte_Code_Converter_Control(Ast_Node_Block*, Storage*);

        void generateByteCode();

    };

}

#endif