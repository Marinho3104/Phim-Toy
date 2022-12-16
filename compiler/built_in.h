#pragma once 

#ifndef COMPILER_BUILT_IN_H
#define COMPILER_BUILT_IN_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Struct_Declaration;
    struct Ast_Node_Name_Space;
    struct Ast_Node_Expression;
    struct Name_Space_Control;
    struct Tokenizer_Control;
    struct Ast_Control;


    extern Name_Space_Control* built_in_name_space_control;
    extern Name_Space_Control* name_space_control;
    extern Tokenizer_Control* tokenizer_control;
    extern Ast_Control* built_in_ast_control;
    extern Ast_Control* ast_control;

}

namespace parser_helper {

    struct Expression_Variable_Declaration;
    struct Type_Information;

}

namespace compiler {

    struct Built_In {

        ~Built_In(); Built_In();

        char* getBuiltInCode();

        void generate();

        static parser::Ast_Node_Function_Declaration* getFunctionDeclaration(parser_helper::Expression_Variable_Declaration*);

        static parser_helper::Type_Information* getReturnTypeOfExpression(parser_helper::Expression_Variable_Declaration*);    

    };

}

#endif