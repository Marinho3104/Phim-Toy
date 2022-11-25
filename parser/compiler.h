#pragma once

#ifndef PARSER_COMPILER_H
#define PARSER_COMPILER_H

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Struct_Declaration;
    struct Ast_Node_Code_Block;
    struct Compiler_Control;
    struct Storage;

    struct Compiler_Declarations {

        utils::LinkedList <Ast_Node_Function_Declaration*>* functions_declaraction;
        utils::LinkedList <Ast_Node_Struct_Declaration*>* structs_declaration;
        utils::LinkedList <Ast_Node_Variable_Declaration*>* variable_declaration;

        ~Compiler_Declarations();

        Compiler_Declarations();

        bool isDeclared(int);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);

        Ast_Node_Function_Declaration* getFunctionDeclaration(int);

        Ast_Node_Struct_Declaration* getStructDeclaration(int);

    };

    struct Compiler_Code_Block {

        utils::LinkedList <byte_code::Byte_Code*>* byte_code;
        Compiler_Declarations* compiler_declarations;
        /* Destructor */
        ~Compiler_Code_Block();

        /* Constructor */
        Compiler_Code_Block(utils::LinkedList <byte_code::Byte_Code*>*);

        /* Generator */
        static void generate(Compiler_Control*, Ast_Node_Code_Block*);

    };

    struct Compiler_Exception {
        const char* description;
        Compiler_Exception(const char*); 
    };

    struct Compiler_Control {

        utils::LinkedList <Compiler_Code_Block*>* compiled_code_blocks;
        Compiler_Code_Block* current_compiler_code_block;

        utils::LinkedList <Ast_Node_Code_Block*>* code_blocks;
        Storage* storage;

        ~Compiler_Control();

        Compiler_Control(utils::LinkedList <Ast_Node_Code_Block*>*, Storage*);

        /* Generator */
        void generateByteCodeBlocks(); 

    };

}

#endif