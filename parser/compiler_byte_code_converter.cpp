#include "./compiler_byte_code_converter.h"

#include "./../byteCode/byte_code_definitions.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./compiler.h"
#include "./ast.h"

#include <iostream>


utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNode(Ast_Node* __node, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    utils::LinkedList <byte_code::Byte_Code*>* _ = new utils::LinkedList <byte_code::Byte_Code*>();

    std::cout << "Node -> " << __node->node_id << std::endl;

    switch (__node->node_id)
    {

        case AST_NODE_VARIABLE_DECLARATION:

            _->add(
                parser::getByteCodeFromVariableDeclaration(
                    (Ast_Node_Variable_Declaration*) __node, __crrnt, __comCntrl
                )
            ); break;

        case AST_NODE_FUNCTION_DECLARATION: 
        
            parser::getByteCodeFromFunctionDeclaration(
                (Ast_Node_Function_Declaration*) __node,
                __crrnt, __comCntrl
            );

            break;

        default: break;
    }

    return _;

}

byte_code::Byte_Code* parser::getByteCodeFromVariableDeclaration(Ast_Node_Variable_Declaration* __varDecl, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for Variable Declaration <--");

    if (__crrnt->compiler_declarations->isDeclared(__varDecl->declaration_id)) new Compiler_Exception("Redeclaration of variable");

    __crrnt->compiler_declarations->variables_declarations->add(__varDecl);

    byte_code::Byte_Code* _bcVarDecl = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_bcVarDecl) byte_code::Byte_Code(
        BYTECODE_MEM_STACK_LOAD, __varDecl->variable_type->getByteSize()
    );

    return _bcVarDecl;

}

void parser::getByteCodeFromFunctionDeclaration(Ast_Node_Function_Declaration* __funcDecl, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for Function Declaration <--");

    __funcDecl->body_pos = Compiler_Code_Block::generate(__comCntrl, __funcDecl->function_body, __crrnt);

    __crrnt->compiler_declarations->functions_declarations->add(__funcDecl);

}


