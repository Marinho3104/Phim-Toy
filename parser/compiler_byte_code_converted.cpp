#include "./compiler_byte_code_converted.h"

#include "./../byteCode/byte_code_definitions.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./compiler.h"

#include <iostream>

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeOfNode(Compiler_Control* __compiler_control, Compiler_Code_Block* __current, Ast_Node* __node) {

    utils::LinkedList <byte_code::Byte_Code*>* _byte_code = new utils::LinkedList <byte_code::Byte_Code*>();

    switch (__node->node_id)
    {
        case AST_NODE_VARIABLE_DECLARATION: 
            
            _byte_code->add(
                parser::getByteCodeFromVariableDeclaration(
                    (Ast_Node_Variable_Declaration*) __node,
                    __current, __compiler_control
                )
            );
            
            goto rtr;
        
        default: new Compiler_Exception("Node not supported");
        
    }

rtr:
    return _byte_code;

}

byte_code::Byte_Code* parser::getByteCodeFromVariableDeclaration(Ast_Node_Variable_Declaration* __variable_declaration, Compiler_Code_Block* __current, Compiler_Control* __compiler_control) {

    __compiler_control->printDebugInfo("--> Byte Code From Variable Declaration <--");

    if (__current->compiler_declarations->isDeclared(__variable_declaration->declaration_id)) new Compiler_Exception("Redeclaration of variable");

    __current->compiler_declarations->variable_declarations->add(__variable_declaration);

    byte_code::Byte_Code* _bcVarDecl = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_bcVarDecl) byte_code::Byte_Code(
        BYTECODE_MEM_STACK_LOAD, __variable_declaration->getByteSize()
    );

    __compiler_control->printDebugInfo("--> Byte Code From Variable Declaration End <--");

    return _bcVarDecl;

}


