#include "./compiler.h"

#include "./compiler_byte_code_converter.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"

#include <iostream>

parser::Compiler_Declarations::~Compiler_Declarations() { 
    variables_declarations->destroy_content = 0; functions_declarations->destroy_content = 0; structs_declarations->destroy_content = 0;
    delete variables_declarations; delete functions_declarations; delete structs_declarations; 
}

parser::Compiler_Declarations::Compiler_Declarations() {
    variables_declarations = new utils::LinkedList <Ast_Node_Variable_Declaration*>();
    functions_declarations = new utils::LinkedList <Ast_Node_Function_Declaration*>();
    structs_declarations = new utils::LinkedList <Ast_Node_Struct_Declaration*>();
}

bool parser::Compiler_Declarations::isDeclared(int __decl) {

    return (
        getVariableDeclaration(__decl) || getFunctionDeclaration(__decl) || getStructDeclaration(__decl)
    );

}

parser::Ast_Node_Variable_Declaration* parser::Compiler_Declarations::getVariableDeclaration(int __decl) {

    for (int _ = 0; _ < variables_declarations->count; _++)

        if ((*variables_declarations)[_]->declaration_id == __decl) return (*variables_declarations)[_];

    return NULL;

}

parser::Ast_Node_Function_Declaration* parser::Compiler_Declarations::getFunctionDeclaration(int __decl) {

    for (int _ = 0; _ < functions_declarations->count; _++)

        if ((*functions_declarations)[_]->declaration_id == __decl) return (*functions_declarations)[_];

    return NULL;

}

parser::Ast_Node_Struct_Declaration* parser::Compiler_Declarations::getStructDeclaration(int __decl) {

    for (int _ = 0; _ < structs_declarations->count; _++)

        if ((*structs_declarations)[_]->declaration_id == __decl) return (*structs_declarations)[_];

    return NULL;

}


parser::Compiler_Code_Block::~Compiler_Code_Block() { delete compiler_declarations; }

parser::Compiler_Code_Block::Compiler_Code_Block(Compiler_Code_Block* _prev) : previous_name_space(_prev) { 
    byte_code = new utils::LinkedList <byte_code::Byte_Code*>(); 
    compiler_declarations = new Compiler_Declarations();
}

int parser::Compiler_Code_Block::generate(Compiler_Control* __comCntrl, parser::Ast_Node_Name_Space* __nmSpc, Compiler_Code_Block* __prev) {

    parser::Compiler_Code_Block* _rtr = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block));
    utils::LinkedList <byte_code::Byte_Code*>* _byte_code;
    new (_rtr) parser::Compiler_Code_Block(__prev);


    for (int _ = 0; _ < __nmSpc->declarations->count; _++) {

        _byte_code = parser::getByteCodeFromNode((*__nmSpc->declarations)[_], _rtr, __comCntrl);

        _rtr->byte_code->join(_byte_code);

        _byte_code->destroy_content = 0;

        delete _byte_code;

    }


    __comCntrl->compiled_blocks->add(_rtr);

    return __comCntrl->compiled_blocks->count - 1;

}

int parser::Compiler_Code_Block::generate(Compiler_Control* __comCntrl, parser::Ast_Node_Code_Block* __node, Compiler_Code_Block* __prev) {

    parser::Compiler_Code_Block* _rtr = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block));
    utils::LinkedList <byte_code::Byte_Code*>* _byte_code;
    new (_rtr) parser::Compiler_Code_Block(__prev);
 

    if (__node) {

        for (int _ = 0; _ < __node->body->count; _++) {

            _byte_code = parser::getByteCodeFromNode((*__node->body)[_], _rtr, __comCntrl);

            _rtr->byte_code->join(_byte_code);

            _byte_code->destroy_content = 0;

            delete _byte_code;

        }

    } else return -1;

    __comCntrl->compiled_blocks->add(_rtr);
    
    return __comCntrl->compiled_blocks->count - 1;

}

int parser::Compiler_Code_Block::generate(Compiler_Control* __comCntrl, parser::Ast_Node_Struct_Declaration* __strcDecl, Compiler_Code_Block* __prev) {

    parser::Compiler_Code_Block* _rtr = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block));
    utils::LinkedList <byte_code::Byte_Code*>* _byte_code;
    new (_rtr) parser::Compiler_Code_Block(__prev);

    if (__strcDecl->fields->count || __strcDecl->functions->count) {

        for (int _ = 0; _ < __strcDecl->fields->count; _++) {

            _byte_code = parser::getByteCodeFromNode((*__strcDecl->fields)[_], _rtr, __comCntrl);

            _rtr->byte_code->join(_byte_code);

            _byte_code->destroy_content = 0;

            delete _byte_code;

        }

        for (int _ = 0; _ < __strcDecl->functions->count; _++) {

            _byte_code = parser::getByteCodeFromNode((*__strcDecl->functions)[_], _rtr, __comCntrl);

            _rtr->byte_code->join(_byte_code);

            _byte_code->destroy_content = 0;

            delete _byte_code;

        }

    } else return -1;

    __comCntrl->compiled_blocks->add(_rtr);

    return __comCntrl->compiled_blocks->count - 1;

}

parser::Ast_Node_Variable_Declaration* parser::Compiler_Code_Block::getVariableDeclaration(int __decl) {

    parser::Ast_Node_Variable_Declaration* _ = compiler_declarations->getVariableDeclaration(__decl);

    if (_) return _;

    else if (previous_name_space) return previous_name_space->getVariableDeclaration(__decl);

    return _;

}

parser::Ast_Node_Function_Declaration* parser::Compiler_Code_Block::getFunctionDeclaration(int __decl) {

    parser::Ast_Node_Function_Declaration* _ = compiler_declarations->getFunctionDeclaration(__decl);

    if (_) return _;

    else if (previous_name_space) return previous_name_space->getFunctionDeclaration(__decl);

    return _;

}

parser::Ast_Node_Struct_Declaration* parser::Compiler_Code_Block::getStructDeclaration(int __decl) {

    parser::Ast_Node_Struct_Declaration* _ = compiler_declarations->getStructDeclaration(__decl);

    if (_) return _;

    else if (previous_name_space) return previous_name_space->getStructDeclaration(__decl);

    return _;

}


parser::Compiler_Exception::Compiler_Exception(const char* __desc) : description(__desc) { std::cout << description << std::endl; exit(1); }


parser::Compiler_Control::~Compiler_Control() { delete compiled_blocks; }

parser::Compiler_Control::Compiler_Control(utils::LinkedList <parser::Ast_Node_Name_Space*>* __nmSpcs, utils::LinkedList <char*>* __impl, bool __dbg) 
    : name_spaces(__nmSpcs), implicit_values(__impl), debug_info(__dbg) { compiled_blocks = new utils::LinkedList <Compiler_Code_Block*>(); }

void parser::Compiler_Control::printDebugInfo(char* _) { if (debug_info) std::cout << "Compiler Control  Debug Info: " << _ << std::endl; }

void parser::Compiler_Control::generate() { Compiler_Code_Block::generate(this, name_spaces->last->object, NULL); }

parser::Compiled_Output* parser::Compiler_Control::generateOutPut() {

    parser::Compiled_Output* _rtr;
    utils::LinkedList <Compiled_Code_Block*>* _blocks = new utils::LinkedList <Compiled_Code_Block*>();
    Compiled_Code_Block*  _block;

    for (int _ = 0; _ < compiled_blocks->count; _++) {

        _block = (Compiled_Code_Block*) malloc(sizeof(Compiled_Code_Block));

        new (_block) Compiled_Code_Block(
            (*compiled_blocks)[_]->byte_code
        );

        _blocks->add(
            _block
        );
    
    }

    _rtr = new Compiled_Output(
        _blocks, implicit_values
    );

    return _rtr;

}


parser::Compiled_Code_Block::~Compiled_Code_Block() { delete compiled_code_block; }

parser::Compiled_Code_Block::Compiled_Code_Block(utils::LinkedList <byte_code::Byte_Code*>* __byte_code) : compiled_code_block(__byte_code) {}


parser::Compiled_Output::~Compiled_Output() { delete compiled_code_blocks; delete implicit_values; }

parser::Compiled_Output::Compiled_Output(utils::LinkedList <Compiled_Code_Block*>* __compiled_code_blocks, utils::LinkedList <char*>* __implicit_values) 
    : compiled_code_blocks(__compiled_code_blocks), implicit_values(__implicit_values) {}

void parser::Compiled_Output::printByteCode() {
    
    for (int _ = 0; _ < compiled_code_blocks->count; _++) {

        std::cout << "Code Block: " << _ << "\n" << std::endl;

        for (int __ = 0; __ < (*compiled_code_blocks)[_]->compiled_code_block->count; __++)

            std::cout << "\tByte code -> " << (int) (unsigned char) (*(*compiled_code_blocks)[_]->compiled_code_block)[__]->code << " " << (*(*compiled_code_blocks)[_]->compiled_code_block)[__]->argument << std::endl;

        std::cout << std::endl;

    }


}
