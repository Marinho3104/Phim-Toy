#pragma once

#ifndef COMPILER_H
#define COMPILER_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    struct Tokenizer_Control;
    struct Compiler_Control;
    struct Ast_Control;
    struct Compiled_Output;
    struct Compiler_Code_Block;
    
};

namespace compiler {

    struct Compiler {

        char* code, *built_in_code;

        parser::Tokenizer_Control* tokenizer_control;
        parser::Ast_Control* ast_control;
        parser::Compiler_Control* compiler_control;

        parser::Tokenizer_Control* tokenizer_control_built_in;
        parser::Ast_Control* ast_control_built_in;
        parser::Compiler_Control* compiler_control_built_in;

        utils::LinkedList <parser::Compiler_Code_Block*>* compiled_built_in;
        parser::Compiled_Output* compiled_output, *compiler_output_built_in;

        ~Compiler(); Compiler(char*);

        void getBuiltIn();

        void generateByteCode();

        void runTokenizer(bool);

        void runAst(bool);

        void runCompiler(bool);

    };

}

#endif