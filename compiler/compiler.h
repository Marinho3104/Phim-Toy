#pragma once

#ifndef COMPILER_H
#define COMPILER_H

namespace compiler {

    // Forward
    struct Built_In;


    struct Compiler_Control {

        char* code;

        ~Compiler_Control(); Compiler_Control(char*);

        void generate();

    };

    struct Compiler {

        Built_In* built_in;
        char* code;

        ~Compiler(); Compiler(char*);

        void generate();

        void generateBuiltIn();

    };

}

#endif