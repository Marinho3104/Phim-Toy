#pragma once 

#ifndef COMPILER_BUILT_IN_H
#define COMPILER_BUILT_IN_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

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

    struct Type_Information;

}

namespace compiler {

    struct Built_In {

        ~Built_In(); Built_In();

        char* getBuiltInCode();

        void generate();

        static parser_helper::Type_Information* getReturnTypeOfExpression(parser::Ast_Node_Expression*);    

    };

}

#endif