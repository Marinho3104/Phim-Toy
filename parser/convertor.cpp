#include "./convertor.h"

#include "./../byte_code/byte_code.h"
#include "./../utils/linkedList.h"
#include "./convertor_helper.h"
#include "./ast.h"

#include <iostream>


parser::Convertor::~Convertor() { delete byte_code_blocks; }

parser::Convertor::Convertor(bool __debug_mode) : debug_mode(__debug_mode) {
    byte_code_blocks = new utils::Linked_List <byte_code::Byte_Code_Block*>();
}

void parser::Convertor::printDebugInformation(const char* __info) { if (debug_mode) std::cout << "\tConvert - Debug Mode: " << __info << std::endl; }

void parser::Convertor::generate() { 

    if (debug_mode) std::cout << "\n\t\t --- Convert ---\n" << std::endl;

    setBlocks();

}

void parser::Convertor::setBlocks() {

    byte_code::Byte_Code_Block* _global_byte_code_block = (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));
    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    parser::Ast_Node_Name_Space* _name_space_node;

    for (int _ = 0; _ < ast_control->name_space_nodes->count; _++) {

        _name_space_node = ast_control->name_space_nodes->operator[](_);

        for (int __ = 0; __ < _name_space_node->declarations->count; __++) {

            _temp = parser_helper::getByteCodeFromNode(
                _name_space_node->declarations->operator[](__)
            );

            _byte_code->join(
                _temp
            );

            _temp->destroy_content = 0; delete _temp;

        }

    }

    new (_global_byte_code_block) byte_code::Byte_Code_Block(
        _byte_code
    );

    byte_code_blocks->add(
        _global_byte_code_block
    );

}

void parser::Convertor::setBlock(Ast_Node_Function_Declaration* __function_declaration) {

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;

    byte_code::Byte_Code_Block* _byte_code_block = (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_byte_code_block) byte_code::Byte_Code_Block(
        _byte_code
    );

    __function_declaration->body_position = byte_code_blocks->add(
        _byte_code_block
    );

    Ast_Node_Code_Block* _code_block = __function_declaration->body;

    for (int _ = 0; _ < _code_block->code->count; _++) {

        _temp = parser_helper::getByteCodeFromNode(
            _code_block->code->operator[](_)
        );

        _byte_code->join(
            _temp
        );

        _temp->destroy_content = 0; delete _temp;

    }

}

byte_code::Compiled_Byte_Code* parser::Convertor::getCompiledByteCode() {

    byte_code::Compiled_Byte_Code* _compiled_byte_code = new byte_code::Compiled_Byte_Code(
        byte_code_blocks, ast_control->implicit_values
    );

    return _compiled_byte_code;

}

void parser::setUpConvertor(bool __debug_mode) { convertor = new Convertor(__debug_mode); }

void parser::cleanConvertor() { delete convertor; }





