
// #include "./../parser/byteCodeConverter.h"
// #include "./../byteCode/byteCode.h"
// #include "./../parser/optimizer.h"
// #include "./../parser/ast.h"
// #include "./../objects/phim_int.h"
// #include "./../objects/phim_string.h"
// #include "./../objects/phim_char.h"

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

}

void getByteCode(char* _code) {

    parser::Tokenizer_Control* _tkCntrl = getTokens(_code);

    parser::Ast_Control* _astCntrl = getAst(_tkCntrl->tokens);

}

int main() {

    getByteCode("int j, lk; ++j;");

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














    // int* l;

    // std::cout << l << std::endl;

    // return 0;



    

    // vm::Vm* _vmControl = new vm::Vm(
    //     _bcCntrl
    // );

    // _vmControl->execute();