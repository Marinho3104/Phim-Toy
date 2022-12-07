#pragma once

#ifndef COMPILER_H
#define COMPILER_H

namespace compiler {

    // Forward
    struct Built_In;

    struct Compiler {

        Built_In* built_in;
        char* code;

        ~Compiler(); Compiler(char*);

        void generate();

    };

}

#endif