#pragma once

#ifndef COMPILER_H
#define COMPILER_H

namespace parser {

    struct Tokenizer_Control;

}

namespace compiler {

    struct Compiler_Control {

        parser::Tokenizer_Control* tokenizer_control;

        char* code;

        ~Compiler_Control(); Compiler_Control(char*);

        void generate();

    };

    struct Compiler {

        char* code;

        ~Compiler(); Compiler(char*);

        void generate();

        void generateBuiltIn();

    };

}

#endif