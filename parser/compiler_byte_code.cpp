#include "./compiled_byte_code.h"

#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"

#include <iostream>

parser::Byte_Code_Block::Byte_Code_Block(utils::LinkedList <byte_code::Byte_Code*>* _) : byte_code_block(_) {}

parser::Compiled_Byte_Code::Compiled_Byte_Code(
    utils::LinkedList <Byte_Code_Block*>* _blocks, utils::LinkedList <Type_Information*>* _types, utils::LinkedList <char*>* _implicitValues) :
        byte_code_blocks(_blocks), types(_types), implicit_values(_implicitValues) {}

void parser::Compiled_Byte_Code::print() {

    for (int _ = 0; _ < byte_code_blocks->count; _++) {

        std::cout << "--> Code Block <--" << std::endl;

        for (int __ = 0; __ < (*byte_code_blocks)[_]->byte_code_block->count; __++)

            std::cout << "Byte code -> " << (int) (unsigned char) (*(*byte_code_blocks)[_]->byte_code_block)[__]->code << " " << (*(*byte_code_blocks)[_]->byte_code_block)[__]->argument << std::endl;

        std::cout << "--> Code Block End <--" << std::endl;

    }

    std::cout << "-------------------" << std::endl;

}



