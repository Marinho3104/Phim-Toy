#pragma once

#ifndef PARSER_CONVERTOR_H
#define PARSER_CONVERTOR_H

#include "./ast_nodes.h"

namespace byte_code { struct Byte_Code; struct Byte_Code_Block; struct Compiled_Byte_Code; }
namespace utils { template <typename> struct Linked_List; }

namespace parser {

    // Forward
    struct Ast_Node_Function_Declaration;
    struct Ast_Control;
    struct Convertor;

    extern Ast_Control* ast_control;
    extern Convertor* convertor;

    struct Convertor {

        utils::Linked_List <byte_code::Byte_Code_Block*>* byte_code_blocks;
        bool debug_mode;

        ~Convertor(); Convertor(bool);

        void printDebugInformation(const char*);

        void generate();

        void setBlocks();

        void setBlock(Ast_Node_Function_Declaration*);

        byte_code::Compiled_Byte_Code* getCompiledByteCode();

    };

    void setUpConvertor(bool);

    void cleanConvertor();


}

#endif