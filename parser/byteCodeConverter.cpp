#include "./byteCodeConverter.h"

#include "./byteCodeConverterHelper.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./astNodes.h"
#include "./ast.h"

#include <iostream>

parser::Byte_Code_Converter_Block::Byte_Code_Converter_Block(int _id, utils::LinkedList <byte_code::Byte_Code*>* _byteCode) : id(_id), byteCode(_byteCode) {}

parser::Byte_Code_Converter_Block* parser::Byte_Code_Converter_Block::generate(Byte_Code_Converter_Control* _bcCnvrtCntrl, int* _crrntNode) {

    utils::LinkedList <byte_code::Byte_Code*>* _byteCode = new utils::LinkedList <byte_code::Byte_Code*>();

    for (; *_crrntNode < _bcCnvrtCntrl->ast->body->count; (*_crrntNode)++) 

        _byteCode->join(
            parser_helper::getByteCodeFromNode( // send pos to function to be added by number of nodes done
                (*_bcCnvrtCntrl->ast->body)[*_crrntNode],
                _bcCnvrtCntrl
            )
        );

    for (int _ = 0; _ < _byteCode->count; _++)

        std::cout << "Byte code -> " << (int) (unsigned char) (*_byteCode)[_]->code << " " << (*_byteCode)[_]->argument << std::endl;

}


parser::Byte_Code_Converter_Control_Exception::Byte_Code_Converter_Control_Exception(char* _description) : description(_description) {

    std::cout << "Error : " << _description << std::endl;
    exit(-1);

}


parser::Byte_Code_Converter_Control::Byte_Code_Converter_Control(Ast_Node_Block* _ast, Storage* _storage) : ast(_ast), storage(_storage) {
    byteCodeBlocks = new utils::LinkedList <Byte_Code_Converter_Block*>();
}

void parser::Byte_Code_Converter_Control::generateByteCode() {

    int _ = 0;

    byteCodeBlocks->add(
        parser::Byte_Code_Converter_Block::generate(
            this, &_
        )
    );

}