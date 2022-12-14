#include "./convertor.h"

#include "./../byte_code/byte_code.h"
#include "./../utils/linkedList.h"
#include "./convertor_helper.h"
#include "./ast.h"

#include <iostream>


parser::Convertor::~Convertor() { delete ast_control; }

parser::Convertor::Convertor(Ast_Control* __ast_control, utils::Linked_List <byte_code::Byte_Code_Block*>* __built_ins, bool __debug_info) 
    : ast_control(__ast_control), built_in_blocks(__built_ins), debug_info(__debug_info) 
        { byte_code_blocks = new utils::Linked_List <byte_code::Byte_Code_Block*>(); }

void parser::Convertor::printDebugInformation(const char* __debug_information) { if (debug_info) std::cout << "Convertor - Debug Info: " << __debug_information << std::endl; }

void parser::Convertor::generate() {

    if (debug_info) std::cout << "\n\t\t--- Convertor ---\n" << std::endl;

    utils::Linked_List <byte_code::Byte_Code*>* _block = new utils::Linked_List <byte_code::Byte_Code*>();

    byte_code::Byte_Code_Block* _byte_code_block = (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_byte_code_block) byte_code::Byte_Code_Block(
        _block
    );

    byte_code_blocks->add(
        _byte_code_block
    );

    for (int _ = 0; _ < ast_control->name_space_nodes->count; _++)

        setGlobalBlock(ast_control->name_space_nodes->operator[](_));

}

void parser::Convertor::setGlobalBlock(Ast_Node_Name_Space* __name_space_node) {

    utils::Linked_List <byte_code::Byte_Code*>* _block = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    
    for (int _ = 0; _ < __name_space_node->declarations->count; _++) {

        _temp = parser_helper::getByteCodeFromNode(__name_space_node->declarations->operator[](_), this);

        _block->join(_temp);

        _temp->destroy_content = 0; delete _temp;

    }

    byte_code_blocks->operator[](0)->block->join(
        _block
    );

    _block->destroy_content = 0; delete _block;

}

void parser::Convertor::addBlock(Ast_Node_Code_Block* __code_block_node) {

    std::cout << "add block" << std::endl;

}

byte_code::Compiled_Byte_Code* parser::Convertor::getCompiledByteCode() {

    byte_code::Compiled_Byte_Code* _compiled_byte_code = new byte_code::Compiled_Byte_Code(
        byte_code_blocks, ast_control->implicit_values
    );

    return _compiled_byte_code;

}




