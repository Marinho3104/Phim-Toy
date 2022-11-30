#pragma once

#ifndef PARSER_COMPILER_H
#define PARSER_COMPILER_H

#include "./ast_nodes.h"

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Compiler_Control;

    struct Compiler_Declarations {

        utils::LinkedList <Ast_Node_Variable_Declaration*>* variables_declarations;
        utils::LinkedList <Ast_Node_Function_Declaration*>* functions_declarations;
        utils::LinkedList <Ast_Node_Struct_Declaration*>* structs_declarations;

        ~Compiler_Declarations(); Compiler_Declarations();

        bool isDeclared(int);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);
        
        Ast_Node_Function_Declaration* getFunctionDeclaration(int);
        
        Ast_Node_Struct_Declaration* getStructDeclaration(int);

    };

    struct Compiler_Code_Block {

        utils::LinkedList <byte_code::Byte_Code*>* byte_code;
        Compiler_Declarations* compiler_declarations;
        Compiler_Code_Block* previous_name_space;

        ~Compiler_Code_Block(); Compiler_Code_Block();

        static int generate(Compiler_Control*, parser::Ast_Node_Name_Space*, Compiler_Code_Block*);

        static int generate(Compiler_Control*, parser::Ast_Node_Code_Block*, Compiler_Code_Block*);

        static int generate(Compiler_Control*, parser::Ast_Node_Struct_Declaration*, Compiler_Code_Block*);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);
        
        Ast_Node_Function_Declaration* getFunctionDeclaration(int);
        
        Ast_Node_Struct_Declaration* getStructDeclaration(int);

    };

    struct Compiler_Exception { const char* description; Compiler_Exception(const char*); };

    struct Compiler_Control {

        utils::LinkedList <parser::Ast_Node_Name_Space*>* name_spaces;
        utils::LinkedList <char*>* implicit_values;

        utils::LinkedList <Compiler_Code_Block*>* compiled_blocks;

        bool debug_info;

        ~Compiler_Control(); Compiler_Control(utils::LinkedList <parser::Ast_Node_Name_Space*>*, utils::LinkedList <char*>*, bool);

        void printDebugInfo(char*);

        void generate();

    };


}

#endif