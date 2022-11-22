#pragma once

#ifndef PARSER_COMPILER_H
#define PARSER_COMPILER_H

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Ast_Node_Function_Declaration; // Ast Node Function Declaration
    struct Ast_Node_Variable_Declaration; // Ast Node Variable Declaration 
    struct Ast_Node_Struct_Declaration; // Ast Node Struct Declaration
    struct Ast_Node_Code_Block; // Ast Node Code Block
    struct Compiler_Control; // Compiler Control
    struct Ast_Node; // Default Node
    struct Storage; // Ast Storage

    struct Compiler_Code_Block {

        utils::LinkedList <byte_code::Byte_Code*>* byte_code;

        /* Constructor */
        Compiler_Code_Block(utils::LinkedList <byte_code::Byte_Code*>*);

        /* Generator */
        static Compiler_Code_Block* generate(Compiler_Control*, Ast_Node_Code_Block*);

    };


    struct Compiler_Control {

        utils::LinkedList <Compiler_Code_Block*>* code_blocks;

        /* Generation only */

        utils::LinkedList <Ast_Node*>* ast;
        utils::LinkedList <Ast_Node_Function_Declaration*>* funcDecl;
        utils::LinkedList <Ast_Node_Struct_Declaration*>* structDecl;
        utils::LinkedList <Ast_Node_Variable_Declaration*>* varDecl;
        Storage* ast_storage;

        /* Generation only */

        /* Constructor */
        Compiler_Control(utils::LinkedList <Ast_Node*>*, Storage*);

        /* Generator */
        void generateByteCode();

    };

}

#endif