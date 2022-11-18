#include "./byteCodeConverter.h"

#include "./byteCodeConverterHelper.h"
// #include "./../byteCode/definitions.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./astNodes.h"
#include "./ast.h"

#include <iostream>

parser::Byte_Code_Converter_Block::Byte_Code_Converter_Block(int _pos, utils::LinkedList <byte_code::Byte_Code*>* _byteCode) : pos(_pos), byteCode(_byteCode) {}

parser::Byte_Code_Converter_Block* parser::Byte_Code_Converter_Block::generate(Byte_Code_Converter_Control* _bcCnvrtCntrl, Ast_Node_Block* _crrntBlock, int _pos) {

    utils::LinkedList <byte_code::Byte_Code*>* _byteCode = new utils::LinkedList <byte_code::Byte_Code*>();
    parser::Byte_Code_Converter_Block* _rtr = (parser::Byte_Code_Converter_Block*) malloc(sizeof(parser::Byte_Code_Converter_Block));

    int _crrntNode = 0;

    for (; _crrntNode < _crrntBlock->body->count; _crrntNode++) { // TODO Function declaraction

        if (
            (*_crrntBlock->body)[_crrntNode]->type == AST_NODE_FUNCTION_DECLARATION
        ) {

            Ast_Node_Function_Declaration* _funcDecl = (Ast_Node_Function_Declaration*) (*_crrntBlock->body)[_crrntNode];

            _bcCnvrtCntrl->byteCodeBlocks->add(
                parser::Byte_Code_Converter_Block::generate(
                    _bcCnvrtCntrl, 
                    (Ast_Node_Block*) _funcDecl->body, 
                    _funcDecl->namePos
                )
            );

            byte_code::Byte_Code* _sep = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

            new(_sep) byte_code::Byte_Code(BYTECODE_IG, 0);

            _bcCnvrtCntrl->byteCodeBlocks->last->object->byteCode->addFrst(_sep);

            for (int _ = _funcDecl->parameters->count - 1; _ >= 0; _--) {

                utils::LinkedList <byte_code::Byte_Code*>* _temp = parser_helper::getByteCodeFromNode((*_funcDecl->parameters)[_], _bcCnvrtCntrl);

                for (int __ = _temp->count - 1; __ >= 0; __--)

                    _bcCnvrtCntrl->byteCodeBlocks->last->object->byteCode->addFrst(
                        (*_temp)[__]
                    );

            }

        }

        else if (
            (*_crrntBlock->body)[_crrntNode]->type == AST_NODE_STRUCT_DECLARATION
        ) {

            Ast_Node_Struct_Declaration* _strcDecl = (Ast_Node_Struct_Declaration*) (*_crrntBlock->body)[_crrntNode];

            _bcCnvrtCntrl->byteCodeBlocks->add(
                parser::Byte_Code_Converter_Block::generate(
                    _bcCnvrtCntrl, 
                    (Ast_Node_Block*) _strcDecl->body, 
                    _strcDecl->namePos
                )
            );            

        }

        else {

            _byteCode->join(
                parser_helper::getByteCodeFromNode(
                    (*_crrntBlock->body)[_crrntNode],
                    _bcCnvrtCntrl
                )
            );
        
        }

    }

    new(_rtr) parser::Byte_Code_Converter_Block(
        _bcCnvrtCntrl->byteCodeBlocks->count,
        _byteCode
    );


    return _rtr;

}


parser::Byte_Code_Converter_Control_Exception::Byte_Code_Converter_Control_Exception(char* _description) : description(_description) {

    std::cout << "Error : " << _description << std::endl;
    exit(-1);

}


parser::Byte_Code_Converter_Control::Byte_Code_Converter_Control(Ast_Node_Block* _ast, Storage* _storage) : ast(_ast), storage(_storage) {
    byteCodeBlocks = new utils::LinkedList <Byte_Code_Converter_Block*>();
}

void parser::Byte_Code_Converter_Control::generateByteCode() {

    byteCodeBlocks->add(
        parser::Byte_Code_Converter_Block::generate(
            this, ast, 0
        )
    );

}