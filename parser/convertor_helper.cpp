#include "./convertor_helper.h"

#include "./../byte_code/byte_code_definitions.h"
#include "./../byte_code/byte_code.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./convertor.h"

#include <iostream>

utils::Linked_List <byte_code::Byte_Code*>* parser_helper::getByteCodeFromNode(parser::Ast_Node* __node, parser::Convertor* __convertor) {

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>();

    switch (__node->node_id)
    {
    case AST_NODE_VARIABLE_DECLARATION:
        
        _byte_code->add(
            getByteCodeFromNodeVariableDeclaration(
                (parser::Ast_Node_Variable_Declaration*) __node,
                __convertor
            )
        );

        break;
    
    default:
        break;
    }

    return _byte_code;

}

byte_code::Byte_Code* parser_helper::getByteCodeFromNodeVariableDeclaration(parser::Ast_Node_Variable_Declaration* __varible_declaration_node, parser::Convertor* __convertor) {

    __convertor->printDebugInformation("Byte Code From Node Variable Declaration Created");

    byte_code::Byte_Code* _byte_code_memory_stack_load = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code_memory_stack_load) byte_code::Byte_Code(
        BYTE_CODE_MEMORY_STACK_LOAD, __varible_declaration_node->getByteSize()
    );

    __convertor->printDebugInformation("Byte Code From Node Variable Declaration End ");

    return _byte_code_memory_stack_load;

}

void parser_helper::getByteCodeFromNodeFunctionDeclaration(parser::Ast_Node_Function_Declaration* __function_declaration_node, parser::Convertor* __convertor) {

    __convertor->printDebugInformation("Byte Code From Node Function Declaration Created");

    


    __convertor->printDebugInformation("Byte Code From Node Function Declaration End");

}

