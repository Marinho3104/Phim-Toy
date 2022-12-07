#pragma once

#ifndef PARSER_COMPILER_AST_H
#define PARSER_COMPILER_AST_H

#include "./ast_nodes.h"

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Ast_Node_Name_Space;
    struct Compiler_Control;
    struct Compiled_Output;
    
    struct Compiler_Declarations {

        utils::LinkedList <Ast_Node_Variable_Declaration*>* variable_declarations;
        utils::LinkedList <Ast_Node_Function_Declaration*>* function_declarations;
        utils::LinkedList <Ast_Node_Struct_Declaration*>* struct_declarations;

        ~Compiler_Declarations(); Compiler_Declarations();

        bool isDeclared(int);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);
        
        Ast_Node_Function_Declaration* getFunctionDeclaration(int);
        
        Ast_Node_Struct_Declaration* getStructDeclaration(int);

    };

    struct Compiler_Code_Block {

        utils::LinkedList <byte_code::Byte_Code*>* byte_code;
        Compiler_Declarations* compiler_declarations;
        Compiler_Code_Block* previous;
        Name_Space* name_space;

        ~Compiler_Code_Block(); Compiler_Code_Block(Name_Space*, Compiler_Code_Block*);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);
        
        Ast_Node_Function_Declaration* getFunctionDeclaration(int);
        
        Ast_Node_Struct_Declaration* getStructDeclaration(int);

        static void generate(Compiler_Control*, parser::Ast_Node_Name_Space*, Compiler_Code_Block*);

        static int generate(Compiler_Control*, parser::Ast_Node_Function_Declaration*, Compiler_Code_Block*, int);

        static void generate(Compiler_Control*, parser::Ast_Node_Struct_Declaration*, Compiler_Code_Block*);

    };

    struct Compiler_Exception { const char* description; Compiler_Exception(const char*); };

    struct Compiler_Control {

        utils::LinkedList <Compiler_Code_Block*>* compiler_code_blocks, *built_in_code_blocks;

        utils::LinkedList <Ast_Node_Name_Space*>* name_space_nodes;
        utils::LinkedList <char*>* implicit_values;

        bool debug_info;

        ~Compiler_Control(); 
        
        Compiler_Control(utils::LinkedList <Ast_Node_Name_Space*>*, utils::LinkedList <char*>*, utils::LinkedList <Compiler_Code_Block*>*, bool);

        Compiler_Control(utils::LinkedList <Ast_Node_Name_Space*>*, utils::LinkedList <char*>*);

        void printDebugInfo(const char*);

        void generate();

        Compiler_Code_Block* getCompilerCodeBlockFromNameSpace(Name_Space*);

        Compiled_Output* generateOutPut(); 

    };

    struct Compiled_Code_Block {
        utils::LinkedList <byte_code::Byte_Code*>* byte_code;
        ~Compiled_Code_Block(); Compiled_Code_Block(utils::LinkedList <byte_code::Byte_Code*>*);
    };

    struct Compiled_Output {

        utils::LinkedList <Compiled_Code_Block*>* compiled_code_blocks;
        utils::LinkedList <char*>* implicit_values;

        ~Compiled_Output(); Compiled_Output(utils::LinkedList <Compiled_Code_Block*>*, utils::LinkedList <char*>*);

        void printByteCode();

    };

}

#endif
