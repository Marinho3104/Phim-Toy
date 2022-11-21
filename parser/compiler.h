#pragma once

#ifndef PARSER_COMPILER_H
#define PARSER_COMPILER_H

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Compiler_Control; // Compiler Control
    struct Ast_Node; // Default Node
    struct Storage; // Ast Storage

    struct Compiler_Code_Block {

        utils::LinkedList <byte_code::Byte_Code*>* byte_code;

        /* Constructor */
        Compiler_Code_Block(utils::LinkedList <byte_code::Byte_Code*>*);

        /* Generator */
        static Compiler_Code_Block* generate(Compiler_Control*, int);

    };


    struct Compiler_Control {

        utils::LinkedList <Compiler_Code_Block*>* code_blocks;
        utils::LinkedList <Ast_Node*>* ast;
        Storage* ast_storage;

        /* Constructor */
        Compiler_Control(utils::LinkedList <Ast_Node*>*, Storage*);

        /* Generator */
        void generateByteCode();

    };

}

#endif