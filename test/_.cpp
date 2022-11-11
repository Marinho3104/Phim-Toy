
#include "./../parser/byteCodeConverter.h"
#include "./../parser/tokenizer.h"
#include "./../utils/linkedList.h"
#include "./../parser/token.h"
#include "./../parser/ast.h"

#include <iostream>

int main() {

    parser::Tokenizer_Control* _tkCntrl = new parser::Tokenizer_Control("int l = 12 + 15 * 123;");

    _tkCntrl->generateTokens();

    parser::Ast_Control* _astCntrl = new parser::Ast_Control(_tkCntrl->tokens);

    _astCntrl->generateAst();

    std::cout << "-------------------" << std::endl;

    parser::Byte_Code_Converter_Control* _bcCntrl = new parser::Byte_Code_Converter_Control(
        _astCntrl->globalBlock, _astCntrl->globalStorage
    );

    _bcCntrl->generateByteCode();

}