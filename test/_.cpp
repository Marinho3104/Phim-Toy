
// #include "./../parser/byteCodeConverter.h"
// #include "./../byteCode/byteCode.h"
// #include "./../parser/optimizer.h"
// #include "./../parser/ast.h"
// #include "./../objects/phim_int.h"
// #include "./../objects/phim_string.h"
// #include "./../objects/phim_char.h"

// #include "./../parser/compiled_byte_code.h" // 

#include "./../byteCode/byteCode.h" // Byte Code

// #include "./../parser/compiler.h" // Compiler

#include "./../parser/tokenizer.h" // Tokenizer
#include "./../parser/token.h" // Tokens

// #include "./../parser/ast_nodes.h" // Ast Nodes
#include "./../parser/ast.h" // Ast control 

#include "./../utils/linkedList.h" // Linked list

#include "./../vm/vm_stdlib.h"
#include "./../vm/memory.h"
#include "./../vm/vm.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>



parser::Tokenizer_Control* getTokens(char* _code) {

    parser::Tokenizer_Control* _tkCntrl = new parser::Tokenizer_Control(_code);

    _tkCntrl->generateTokens();

    std::cout << "\n --> Generate Tokens <--\n" << std::endl;

    for (int _ = 0; _ < _tkCntrl->tokens->count; _++)

        std::cout << "Token id -> " << (*_tkCntrl->tokens)[_]->id << std::endl;

    return _tkCntrl;

}

parser::Ast_Control* getAst(utils::LinkedList <parser::Token*>* tokensColl) {

    std::cout << "\n --> Generate Ast <--\n" << std::endl;

    parser::Ast_Control* _astCntrl = new parser::Ast_Control(tokensColl, 1);

    _astCntrl->generate();

    return _astCntrl;

}

// parser::Compiler_Control* getCompilerControl(utils::LinkedList <parser::Ast_Node_Name_Space*>* _name_spaces, parser::Storage* _storage) {

//     std::cout << "\n --> Generate Compiler <--\n" << std::endl;

//     parser::Compiler_Control* _comCntrl = new parser::Compiler_Control(_name_spaces, _storage->implicit_values, 1);

//     _comCntrl->generate();

//     return _comCntrl;

// }

void getByteCode(char* _code) {

    parser::Tokenizer_Control* _tkCntrl = getTokens(_code);

    parser::Ast_Control* _astCntrl = getAst(_tkCntrl->tokens);
    
    delete _tkCntrl;

    // parser::Compiler_Control* _comCntrl = getCompilerControl(_astCntrl->name_spaces, _astCntrl->storage);

    delete _astCntrl;

    // parser::Compiled_Output* _ = _comCntrl->generateOutPut();

    // delete _comCntrl;

    // std::cout << "\n --> Generate Byte Code <--\n" << std::endl;

    // _->printByteCode();

    // return _;

}

int k; 


int main() {

    "void jk(int l) { void k; }" \
    "int kk() {} " \
    "jk(kk());";
    "int j; {int k; { int kl; j; } } { int j; int ll; } j += 12 + 12 * 5;";
    "namespace ola {} :: ola ::";
    "namespace ola { void gg(); } ola::gg() {}";
    "int l; namespace adeus {  struct test { int j; }; int oo = 12; int mario = oo; } namespace ola { int marinho; void mormormor(int l = 12) { *::adeus::mario = (::adeus::oo); }  } int o; " \
    "void main() { adeus::test* l; } void ::ola::test::jk(int j, int k) {} ";
    "int k; namespace ola {  int mamamia; struct test { int k; int oll = k; void jk(int j, int k); }; } " \
    "int main() { ola::test* var; }";
    "int jk; struct ola { int jk; void olak() { jk; } }; int* test; void testFunc(int j) {}" \
    "void main() { testFunc(12); } ";
        "int jk; struct ola { int jk, qw, ol; void olak() { jk; } }; int* test; void testFunc(int j) {}" \
        "void main() { ola meu; ::ola::olak(meu); } ";
    getByteCode(
        "int j; namespace ola {int k; } int ola; " \
        ""
    );

    // delete _com;

    // executeByteCode(_com);

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











/*



parser::Compiled_Byte_Code* getCompilerControl(utils::LinkedList <parser::Ast_Node*>* _ast, parser::Storage* _storage) {

    parser::Compiler_Control* _comCntrl = new parser::Compiler_Control(_ast, _storage);

    _comCntrl->generateByteCode();

    parser::Compiled_Byte_Code* _rtr = _comCntrl->getCompiledByteCode();

    _rtr->print();

    return _rtr;

}

void executeByteCode(parser::Compiled_Byte_Code* _toExe) {

    vm::Vm* _vm = (vm::Vm*) malloc(sizeof(vm::Vm));

    new (_vm) vm::Vm();

    _vm->execute(
        _toExe
    );

}


*/