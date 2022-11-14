
#include "./../parser/byteCodeConverter.h"
#include "./../byteCode/byteCode.h"
#include "./../parser/tokenizer.h"
#include "./../parser/optimizer.h"
#include "./../utils/linkedList.h"
#include "./../parser/token.h"
#include "./../parser/ast.h"
#include "./../objects/phim_int.h"

#include <iostream>

int main() {

    objects::Phim_Int* _ = objects::PhimIntFromCString("1234");

    objects::Phim_Object* __ = _;

    std::cout << "pass" << std::endl;
    __->bitsRepresentation();

    // _->bitsRepresentation();

    return 0;

    parser::Tokenizer_Control* _tkCntrl = new parser::Tokenizer_Control("void l(int j) { int k; } l(12 + 12, 15); struct lk { int l; }; lk ola;");

    _tkCntrl->generateTokens();

    for (int _ = 0; _ < _tkCntrl->tokens->count; _++)

        std::cout << "Token id -> " << (*_tkCntrl->tokens)[_]->id << std::endl;

    std::cout << "-------------------" << std::endl;

    parser::Ast_Control* _astCntrl = new parser::Ast_Control(_tkCntrl->tokens);

    _astCntrl->generateAst();

    // parser::optimizerNames(_astCntrl->globalStorage->names);

    std::cout << "-------------------" << std::endl;

    parser::Byte_Code_Converter_Control* _bcCntrl = new parser::Byte_Code_Converter_Control(
        _astCntrl->globalBlock, _astCntrl->globalStorage
    );

    _bcCntrl->generateByteCode();

    for (int _ = 0; _ < _bcCntrl->byteCodeBlocks->count; _++) {

        std::cout << "--> Code Block <--" << std::endl;

        for (int __ = 0; __ < (*_bcCntrl->byteCodeBlocks)[_]->byteCode->count; __++)

            std::cout << "Byte code -> " << (int) (unsigned char) (*(*_bcCntrl->byteCodeBlocks)[_]->byteCode)[__]->code << " " << (*(*_bcCntrl->byteCodeBlocks)[_]->byteCode)[__]->argument << std::endl;

        std::cout << "--> Code Block End <--" << std::endl;

    }

}