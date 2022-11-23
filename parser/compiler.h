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
    struct Compiled_Byte_Code; // Compiler Byte Code
    struct Compiler_Control; // Compiler Control
    struct Ast_Node; // Default Node
    struct Storage; // Ast Storage

    struct Compiler_Declarations {

        utils::LinkedList <Ast_Node_Function_Declaration*>* funcDecl;
        utils::LinkedList <Ast_Node_Struct_Declaration*>* structDecl;
        utils::LinkedList <Ast_Node_Variable_Declaration*>* varDecl;

        Compiler_Declarations();

    };

    struct Compiler_Code_Block {

        utils::LinkedList <byte_code::Byte_Code*>* byte_code;

        /* Generation only */

        Compiler_Declarations* compilerDeclarations;

        /* Generation only */

        /* Constructor */
        Compiler_Code_Block(utils::LinkedList <byte_code::Byte_Code*>*);

        /* Generator */
        static Compiler_Code_Block* generate(Compiler_Control*, Ast_Node_Code_Block*, Compiler_Declarations*);

    };


    struct Compiler_Control {

        utils::LinkedList <Compiler_Code_Block*>* code_blocks;
        Compiler_Code_Block* crrntCompileCodeBlock;

        utils::LinkedList <Ast_Node*>* ast;
        Storage* ast_storage;

        /* Constructor */
        Compiler_Control(utils::LinkedList <Ast_Node*>*, Storage*);

        /* Generator */
        void generateByteCode();

        /* Get Compiler Byte Code */
        Compiled_Byte_Code* getCompiledByteCode();

    };

}

#endif