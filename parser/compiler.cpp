#include "./compiler.h"

#include "./compiler_byte_code_converted.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"

#include <iostream>

parser::Compiler_Declarations::~Compiler_Declarations() { 
    variable_declarations->destroy_content = 0; function_declarations->destroy_content = 0; struct_declarations->destroy_content = 0;
    delete variable_declarations; delete function_declarations; delete struct_declarations; 
}

parser::Compiler_Declarations::Compiler_Declarations() {
    variable_declarations = new utils::LinkedList <Ast_Node_Variable_Declaration*>();
    function_declarations = new utils::LinkedList <Ast_Node_Function_Declaration*>();
    struct_declarations = new utils::LinkedList <Ast_Node_Struct_Declaration*>();
}

bool parser::Compiler_Declarations::isDeclared(int __declaration_id) {

    return (
        getVariableDeclaration(__declaration_id) || getFunctionDeclaration(__declaration_id) || getStructDeclaration(__declaration_id)
    );

}

parser::Ast_Node_Variable_Declaration* parser::Compiler_Declarations::getVariableDeclaration(int __declaration_id) {

    for (int _ = 0; _ < variable_declarations->count; _++)

        if ((*variable_declarations)[_]->declaration_id == __declaration_id) return (*variable_declarations)[_];

    return NULL;

}

parser::Ast_Node_Function_Declaration* parser::Compiler_Declarations::getFunctionDeclaration(int __declaration_id) {

    for (int _ = 0; _ < function_declarations->count; _++)

        if ((*function_declarations)[_]->declaration_id == __declaration_id) return (*function_declarations)[_];

    return NULL;

}

parser::Ast_Node_Struct_Declaration* parser::Compiler_Declarations::getStructDeclaration(int __declaration_id) {

    for (int _ = 0; _ < struct_declarations->count; _++)

        if ((*struct_declarations)[_]->declaration_id == __declaration_id) return (*struct_declarations)[_];

    return NULL;

}


parser::Compiler_Code_Block::~Compiler_Code_Block() { delete byte_code; delete compiler_declarations; }

parser::Compiler_Code_Block::Compiler_Code_Block(Name_Space* __name_space) : name_space(__name_space) {
    byte_code = new utils::LinkedList <byte_code::Byte_Code*>();
    compiler_declarations = new Compiler_Declarations();
}

int parser::Compiler_Code_Block::generate(Compiler_Control* __compiler_control, parser::Ast_Node_Name_Space* __name_space_node) {

    __compiler_control->printDebugInfo("--> Compiler Code Block <--");

    parser::Compiler_Code_Block* _compiler_code_block = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block));
    new (_compiler_code_block) parser::Compiler_Code_Block(__name_space_node->name_space);

    utils::LinkedList <byte_code::Byte_Code*>* _byte_code;

    int _pos = __compiler_control->compiled_code_blocks->add(_compiler_code_block);

    for (int _ = 0; _ <  __name_space_node->declarations->count; _++) {

        _byte_code = parser::getByteCodeOfNode(__compiler_control, _compiler_code_block, (*__name_space_node->declarations)[_]);

        _compiler_code_block->byte_code->join(
            _byte_code
        );

        _byte_code->destroy_content = 0; delete _byte_code;

    }

    __compiler_control->printDebugInfo("--> Compiler Code Block End <--");

    return _pos;

}


parser::Compiler_Exception::Compiler_Exception(const char* __description) : description(__description) { std::cout << "Compiler Exception: " << description << std::endl; exit(1); }


parser::Compiler_Control::~Compiler_Control() { delete compiled_code_blocks; }

parser::Compiler_Control::Compiler_Control(
    utils::LinkedList <Ast_Node_Name_Space*>* __name_space_nodes, utils::LinkedList <char*>* __implicit_values, bool __debug_info) 
        : name_space_nodes(__name_space_nodes), implicit_values(__implicit_values), debug_info(__debug_info) {
            compiled_code_blocks = new utils::LinkedList <Compiler_Code_Block*>();
    }

void parser::Compiler_Control::printDebugInfo(const char* __debug_info) { std::cout << "Compiler Control - Debug Info: " << __debug_info << std::endl << std::endl; }

void parser::Compiler_Control::generate() { Compiler_Code_Block::generate(this, name_space_nodes->frst->object); }