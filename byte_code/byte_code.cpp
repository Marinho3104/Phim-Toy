#include "./byte_code.h"

#include "./../utils/linkedList.h"
#include "./../parser/ast.h"

#include <iostream>


byte_code::Byte_Code::~Byte_Code() {}

byte_code::Byte_Code::Byte_Code(char __code, int __argument) : code(__code), argument(__argument) {}


byte_code::Byte_Code_Block::~Byte_Code_Block() { delete block; }

byte_code::Byte_Code_Block::Byte_Code_Block(utils::Linked_List <Byte_Code*>* __block) 
    : block(__block) {}


byte_code::Compiled_Byte_Code::~Compiled_Byte_Code() { /* delete blocks; delete implicit_values;*/ }

byte_code::Compiled_Byte_Code::Compiled_Byte_Code(utils::Linked_List <Byte_Code_Block*>* __blocks, utils::Linked_List <char*>* __implicit_values) 
    : blocks(__blocks), implicit_values(__implicit_values) {}

void byte_code::Compiled_Byte_Code::print() {

    std::cout << "\n\nCompiled Byte Code:\n" << std::endl;

    for (int _ = 0; _ < blocks->count; _++) {

        std::cout << "\tBlock -> " << _ << std::endl << std::endl;

        for (int __ = 0; __ < blocks->operator[](_)->block->count; __++)

            std::cout << "\t\tByte Code -> " << (int) blocks->operator[](_)->block->operator[](__)->code << " " 
                << blocks->operator[](_)->block->operator[](__)->argument << std::endl;

        std::cout << std::endl;

    }


}



