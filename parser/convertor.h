#pragma once


#ifndef PARSER_CONVERTOR_H
#define PARSER_CONVERTOR_H

#include "./ast_nodes.h"

namespace byte_code { struct Byte_Code; struct Byte_Code_Block; struct Compiled_Byte_Code; }
namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Ast_Control;

    struct Convertor {

        utils::Linked_List <byte_code::Byte_Code_Block*>* byte_code_blocks, *built_in_blocks;
        Ast_Control* ast_control;

        bool debug_info;
        
        ~Convertor(); Convertor(Ast_Control*, utils::Linked_List <byte_code::Byte_Code_Block*>*, bool);

        void printDebugInformation(const char*);

        void generate();

        void setGlobalBlock(Ast_Node_Name_Space*);

        void addBlock(Ast_Node_Code_Block*);

        byte_code::Compiled_Byte_Code* getCompiledByteCode();
        
    };

}

#endif