#include "./compiler.h"

#include "./compiler_byte_code_converted.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./ast.h"

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

    std::cout << getVariableDeclaration(__declaration_id) << std::endl;

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


parser::Compiler_Code_Block::~Compiler_Code_Block() { delete compiler_declarations; }

parser::Compiler_Code_Block::Compiler_Code_Block(Name_Space* __name_space, Compiler_Code_Block* __previous) : name_space(__name_space), previous(__previous) {
    byte_code = new utils::LinkedList <byte_code::Byte_Code*>();
    compiler_declarations = new Compiler_Declarations();
}

parser::Ast_Node_Variable_Declaration* parser::Compiler_Code_Block::getVariableDeclaration(int __declaration_id) {

    parser::Ast_Node_Variable_Declaration* _variable_declaration = compiler_declarations->getVariableDeclaration(__declaration_id);

    return _variable_declaration || !previous ? _variable_declaration : previous->getVariableDeclaration(__declaration_id);

}

parser::Ast_Node_Function_Declaration* parser::Compiler_Code_Block::getFunctionDeclaration(int __declaration_id) {

    parser::Ast_Node_Function_Declaration* _function_declaration = compiler_declarations->getFunctionDeclaration(__declaration_id);

    return _function_declaration || !previous ? _function_declaration : previous->getFunctionDeclaration(__declaration_id);

}

parser::Ast_Node_Struct_Declaration* parser::Compiler_Code_Block::getStructDeclaration(int __declaration_id) {

    parser::Ast_Node_Struct_Declaration* _struct_declaration = compiler_declarations->getStructDeclaration(__declaration_id);

    return _struct_declaration || !previous ? _struct_declaration : previous->getStructDeclaration(__declaration_id);

}

void parser::Compiler_Code_Block::generate(Compiler_Control* __compiler_control, parser::Ast_Node_Name_Space* __name_space_node, Compiler_Code_Block* __previous) {

    __compiler_control->printDebugInfo("--> Compiler Code Block <--");

    parser::Compiler_Code_Block* _compiler_code_block = __compiler_control->getCompilerCodeBlockFromNameSpace(__name_space_node->name_space);

    if (!_compiler_code_block) {
        _compiler_code_block = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block));
        new (_compiler_code_block) parser::Compiler_Code_Block(__name_space_node->name_space, __previous);
        __compiler_control->compiled_code_blocks->add(_compiler_code_block);
    } 

    utils::LinkedList <byte_code::Byte_Code*>* _byte_code;

    for (int _ = 0; _ <  __name_space_node->declarations->count; _++) {

        _byte_code = parser::getByteCodeFromNode(__compiler_control, _compiler_code_block, (*__name_space_node->declarations)[_]);

        _compiler_code_block->byte_code->join(
            _byte_code
        );

        _byte_code->destroy_content = 0; delete _byte_code;

    }

    __compiler_control->printDebugInfo("--> Compiler Code Block End <--");

}

int parser::Compiler_Code_Block::generate(Compiler_Control* __compiler_control, parser::Ast_Node_Function_Declaration* __function_declaration, Compiler_Code_Block* __previous, int __position) {

    __compiler_control->printDebugInfo("--> Compiler Code Block -- Function Declaration <--");

    Compiler_Code_Block* _compiler_code_block;

    if (__position != -1) _compiler_code_block = (*__compiler_control->compiled_code_blocks)[__position];

    else {
        _compiler_code_block = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block));
        new (_compiler_code_block) parser::Compiler_Code_Block(__function_declaration->name_space, __previous);
        __position = __compiler_control->compiled_code_blocks->add(_compiler_code_block);
    }

    if (!__function_declaration->body) return __position;

    utils::LinkedList <byte_code::Byte_Code*>* _byte_code;

    for (int _ = 0; _ <  __function_declaration->body->code->count; _++) {

        _byte_code = parser::getByteCodeFromNode(__compiler_control, _compiler_code_block, (*__function_declaration->body->code)[_]);

        _compiler_code_block->byte_code->join(
            _byte_code
        );

        _byte_code->destroy_content = 0; delete _byte_code;

    }

    __compiler_control->printDebugInfo("--> Compiler Code Block -- Function Declaration End <--");

    return __position;
    

}

void parser::Compiler_Code_Block::generate(Compiler_Control* __compiler_control, parser::Ast_Node_Struct_Declaration* __struct_declaration, Compiler_Code_Block* __previous) {

    __compiler_control->printDebugInfo("--> Compiler Code Block -- Struct Declaration <--");

    Compiler_Code_Block* _compiler_code_block = __compiler_control->getCompilerCodeBlockFromNameSpace(__struct_declaration->own_name_space);

    if (!_compiler_code_block) {
        _compiler_code_block = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block));
        new (_compiler_code_block) parser::Compiler_Code_Block(__struct_declaration->own_name_space, __previous);
        __compiler_control->compiled_code_blocks->add(_compiler_code_block);
    } 

    for (int _ = 0; _ < __struct_declaration->functions->count; _++)

        parser::getByteCodeFromFunctionDeclaration(
            (*__struct_declaration->functions)[_], _compiler_code_block, __compiler_control
        );

    __compiler_control->printDebugInfo("--> Compiler Code Block -- Struct Declaration End <--");

}

parser::Compiler_Exception::Compiler_Exception(const char* __description) : description(__description) { std::cout << "Compiler Exception: " << description << std::endl; exit(1); }


parser::Compiler_Control::~Compiler_Control() { delete compiled_code_blocks; }

parser::Compiler_Control::Compiler_Control(
    utils::LinkedList <Ast_Node_Name_Space*>* __name_space_nodes, utils::LinkedList <char*>* __implicit_values, bool __debug_info) 
        : name_space_nodes(__name_space_nodes), implicit_values(__implicit_values), debug_info(__debug_info) {
            compiled_code_blocks = new utils::LinkedList <Compiler_Code_Block*>();
            built_in_code_blocks = new utils::LinkedList <Compiler_Code_Block*>();
            generateBuiltInCodeBlocks();
    }

void parser::Compiler_Control::generateBuiltInCodeBlocks() {

    

}

void parser::Compiler_Control::printDebugInfo(const char* __debug_info) { std::cout << "Compiler Control - Debug Info: " << __debug_info << std::endl << std::endl; }

void parser::Compiler_Control::generate() { 

    for (int _ = 1; _ < name_space_nodes->count; _++) 

        Compiler_Code_Block::generate(this, (*name_space_nodes)[_], NULL);     
    
    Compiler_Code_Block::generate(this, name_space_nodes->frst->object, NULL); 
    
}

parser::Compiled_Output* parser::Compiler_Control::generateOutPut() {

    parser::Compiled_Output* _rtr;
    utils::LinkedList <Compiled_Code_Block*>* _blocks = new utils::LinkedList <Compiled_Code_Block*>();
    Compiled_Code_Block*  _block;

    for (int _ = 0; _ < compiled_code_blocks->count; _++) {

        _block = (Compiled_Code_Block*) malloc(sizeof(Compiled_Code_Block));

        new (_block) Compiled_Code_Block(
            (*compiled_code_blocks)[_]->byte_code
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

parser::Compiler_Code_Block* parser::Compiler_Control::getCompilerCodeBlockFromNameSpace(Name_Space* __name_space) {

    // std::cout << __name_space << std::endl;
    // __name_space->printScope();

    if (!__name_space) return NULL;

    for (int _ = 0; _ < compiled_code_blocks->count; _++)

        if ((*compiled_code_blocks)[_]->name_space == __name_space) return (*compiled_code_blocks)[_];

    return NULL;

}


parser::Compiled_Code_Block::~Compiled_Code_Block() { delete byte_code; }

parser::Compiled_Code_Block::Compiled_Code_Block(utils::LinkedList <byte_code::Byte_Code*>* __byte_code) : byte_code(__byte_code) {}


parser::Compiled_Output::~Compiled_Output() { delete compiled_code_blocks; delete implicit_values; }

parser::Compiled_Output::Compiled_Output(utils::LinkedList <Compiled_Code_Block*>* __compiled_code_blocks, utils::LinkedList <char*>* __implicit_values) 
    : compiled_code_blocks(__compiled_code_blocks), implicit_values(__implicit_values) {}

void parser::Compiled_Output::printByteCode() {
    
    for (int _ = 0; _ < compiled_code_blocks->count; _++) {

        std::cout << "Code Block: " << _ << "\n" << std::endl;

        for (int __ = 0; __ < (*compiled_code_blocks)[_]->byte_code->count; __++)

            std::cout << "\tByte code -> " << (int) (unsigned char) (*(*compiled_code_blocks)[_]->byte_code)[__]->code << " " << (*(*compiled_code_blocks)[_]->byte_code)[__]->argument << std::endl;

        std::cout << std::endl;

    }


}
