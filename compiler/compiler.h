#pragma once

#ifndef COMPILER_H
#define COMPILER_H

namespace parser {

    struct Tokenizer_Control;
    struct Compiler_Control;
    struct Ast_Control;
    struct Compiled_Output;
    struct Compiler_Code_Block;
    
};

namespace utils { template <typename> struct LinkedList; }

namespace compiler {

    struct Compiler {

        char* code, *built_in_code;

        parser::Tokenizer_Control* tokenizer_control;
        parser::Ast_Control* ast_control;
        parser::Compiler_Control* compiler_control;

        utils::LinkedList <parser::Compiler_Code_Block*>* built_in_code_blocks;

        parser::Compiled_Output* compiled_output;

        ~Compiler(); Compiler(char*);

        void getBuiltInCodeBlock();

        void generateByteCode();

        void runTokenizer(bool);

        void runAst(bool);

        void runCompiler(bool);

    };

}

#endif
