
// #include "./../parser/byteCodeConverter.h"
// #include "./../byteCode/byteCode.h"
// #include "./../parser/optimizer.h"
// #include "./../parser/ast.h"
// #include "./../objects/phim_int.h"
// #include "./../objects/phim_string.h"
// #include "./../objects/phim_char.h"

#include "./../byteCode/byteCode.h" // Byte Code

#include "./../parser/compiler.h" // Compiler

#include "./../parser/tokenizer.h" // Tokenizer
#include "./../parser/token.h" // Tokens

#include "./../parser/ast_nodes.h" // Ast Nodes
#include "./../parser/ast.h" // Ast control 

#include "./../utils/linkedList.h" // Linked list

#include "./../vm/vm_stdlib.h"
#include "./../vm/memory.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

parser::Tokenizer_Control* getTokens(char* _code) {

    parser::Tokenizer_Control* _tkCntrl = new parser::Tokenizer_Control(_code);

    _tkCntrl->generateTokens();

    std::cout << "--> Tokens <--" << std::endl;

    for (int _ = 0; _ < _tkCntrl->tokens->count; _++)

        std::cout << "Token id -> " << (*_tkCntrl->tokens)[_]->id << std::endl;

    std::cout << "-------------------" << std::endl;

    return _tkCntrl;

}

parser::Ast_Control* getAst(utils::LinkedList <parser::Token*>* tokensColl) {

    parser::Ast_Control* _astCntrl = new parser::Ast_Control(tokensColl);

    _astCntrl->generateAst();

    std::cout << "-------------------" << std::endl;

    return _astCntrl;

}

parser::Compiler_Control* getCompilerControl(utils::LinkedList <parser::Ast_Node*>* _ast, parser::Storage* _storage) {

    parser::Compiler_Control* _comCntrl = new parser::Compiler_Control(_ast, _storage);

    _comCntrl->generateByteCode();

    for (int _ = 0; _ < _comCntrl->code_blocks->count; _++) {

        std::cout << "--> Code Block <--" << std::endl;

        for (int __ = 0; __ < (*_comCntrl->code_blocks)[_]->byte_code->count; __++)

            std::cout << "Byte code -> " << (int) (unsigned char) (*(*_comCntrl->code_blocks)[_]->byte_code)[__]->code << " " << (*(*_comCntrl->code_blocks)[_]->byte_code)[__]->argument << std::endl;

        std::cout << "--> Code Block End <--" << std::endl;

    }

    std::cout << "-------------------" << std::endl;

    return _comCntrl;

}

void getByteCode(char* _code) {

    parser::Tokenizer_Control* _tkCntrl = getTokens(_code);

    parser::Ast_Control* _astCntrl = getAst(_tkCntrl->tokens);

    parser::Compiler_Control* _comCntrl = getCompilerControl(_astCntrl->code_blocks, _astCntrl->storage);

}

int main() {

    getByteCode(
        "void jk(int) { void k; }" \
        "int kk() {} " \
        "jk(kk());" \
        "jk + 12;"
    );

    return 0;



    vm::Memory* _memory = new vm::Memory();

    int l = 789; 


    address_t _addr;
    
    for (int _ = 0; _ < 100 ; _++)
        _addr = vm_stdlib::allocMemStack(
            &l, 4, _memory
        );

    std::cout << _addr << std::endl;

    _memory->addNewAddress(_addr);

    std::cout << _memory->getAddress(0) << std::endl;

}
