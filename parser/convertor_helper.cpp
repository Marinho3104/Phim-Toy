#include "./convertor_helper.h"

#include "./../byte_code/byte_code_definitions.h"
#include "./../byte_code/byte_code.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./exception_handle.h"
#include "./convertor.h"
#include "./token.h"

#include <iostream>


int parser_helper::getExpressionPriority(int __token_id) {

    if (__token_id >= TOKEN_MULTIPLICATION && __token_id <= TOKEN_MODULUS) return 2;
    if (__token_id >= TOKEN_ADDITION && __token_id <= TOKEN_SUBTRACTION) return 3;
    if (__token_id >= TOKEN_BITWISEAND && __token_id <= TOKEN_BITWISERIGHTSHIFT) return 4;
    if (__token_id >= TOKEN_AND && __token_id <= TOKEN_NOT) return 5;

    return 6;

}

byte_code::Byte_Code* parser_helper::getByteCodeOfExpressionId(int __token_id) {

    byte_code::Byte_Code* _ = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    switch (__token_id)
    {
        case TOKEN_ADDITION: new(_) byte_code::Byte_Code(BYTE_CODE_ADDITION, 0); break;
        case TOKEN_SUBTRACTION: new(_) byte_code::Byte_Code(BYTE_CODE_SUBTRACTION, 0); break;
        case TOKEN_MULTIPLICATION: new(_) byte_code::Byte_Code(BYTE_CODE_MULTIPLICATION, 0); break;
        case TOKEN_DIVISION: new(_) byte_code::Byte_Code(BYTE_CODE_DIVISION, 0); break;
        case TOKEN_MODULUS: new(_) byte_code::Byte_Code(BYTE_CODE_MODULOS, 0); break;
        case TOKEN_ADDITIONASSIGMENT: new(_) byte_code::Byte_Code(BYTE_CODE_ASSIGN_ADDITION, 0); break;
        case TOKEN_SUBTRACTIONASSIGMENT: new(_) byte_code::Byte_Code(BYTE_CODE_ASSIGN_SUBTRACTION, 0); break;
        case TOKEN_MULTIPLICATIONASSIGMENT: new(_) byte_code::Byte_Code(BYTE_CODE_ASSIGN_MULTIPLICATION, 0); break;
        case TOKEN_DIVISIONASSIGMENT: new(_) byte_code::Byte_Code(BYTE_CODE_ASSIGN_DIVISION, 0); break;
        case TOKEN_MODULOSASSIGMENT: new(_) byte_code::Byte_Code(BYTE_CODE_ASSIGN_MODULOS, 0); break;
        case TOKEN_INCREMENT: new(_) byte_code::Byte_Code(BYTE_CODE_ASSIGN_INCREMENT, 0); break;
        case TOKEN_DECREMENT: new(_) byte_code::Byte_Code(BYTE_CODE_ASSIGN_DECREMENT, 0); break;
        case TOKEN_INCREMENT_LEFT: new(_) byte_code::Byte_Code(BYTE_CODE_ASSIGN_INCREMENT_LEFT, 0); break;
        case TOKEN_DECREMENT_LEFT: new(_) byte_code::Byte_Code(BYTE_CODE_ASSIGN_DECREMENT_LEFT, 0); break;
        case TOKEN_EQUAL: new (_) byte_code::Byte_Code(BYTE_CODE_ASSIGN, 0); break;
        case TOKEN_ADDITION_BINARY: new (_) byte_code::Byte_Code(BYTE_CODE_BINARY_ADDITION, 0); break;
        case TOKEN_SUBTRACTION_BINARY: new (_) byte_code::Byte_Code(BYTE_CODE_BINARY_SUBTRACTION, 0); break;
        default: new parser::Exception_Handle("Expression not supported");
    }

    return _;

}

utils::Linked_List <byte_code::Byte_Code*>* parser_helper::getByteCodeFromNode(parser::Ast_Node* __node) {

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>();

    switch (__node->node_id)
    {
    case AST_NODE_STRUCT_DECLARATION: goto _return;
    case AST_NODE_VARIABLE_DECLARATION:
        
        _byte_code->add(
            getByteCodeFromNodeVariableDeclaration(
                (parser::Ast_Node_Variable_Declaration*) __node
            )
        );

        goto _return;

    case AST_NODE_VARIABLE:

        _byte_code->add(
            getByteCodeFromVariable(
                (parser::Ast_Node_Variable*) __node
            )
        );

        goto _return;

    case AST_NODE_VALUE:

        _byte_code->add(
            getByteCodeFromValue(
                (parser::Ast_Node_Value*) __node
            )
        );

        goto _return;

    case AST_NODE_FUNCTION_DECLARATION:

        getByteCodeFromNodeFunctionDeclaration(
            (parser::Ast_Node_Function_Declaration*) __node
        );

        goto _return;

    case AST_NODE_FUNCTION_CALL:

        _byte_code->add(
            getByteCodeFromFunctionCall(
                (parser::Ast_Node_Function_Call*) __node
            )
        );

        goto _return;
    
    default: break;
    }

    delete _byte_code;

    switch (__node->node_id)
    {
    case AST_NODE_EXPRESSION:
        
        _byte_code = getByteCodeFromExpression(
            (parser::Ast_Node_Expression*) __node
        );

        break;

    case AST_NODE_POINTER_OPERATOR:

        _byte_code = getByteCodeFromPointerOperator(
            (parser::Ast_Node_Pointer_Operator*) __node
        );

        break;

    case AST_NODE_PARENTHESIS:

        _byte_code = getByteCodeFromParenthesis(
            (parser::Ast_Node_Parenthesis*) __node
        );

        break;
    
    default: 
        std::cout << __node->node_id << std::endl;
        new parser::Exception_Handle("Error getting node type");
    }

_return:
    return _byte_code;

}

byte_code::Byte_Code* parser_helper::getByteCodeFromNodeVariableDeclaration(parser::Ast_Node_Variable_Declaration* __variable_declaration_node) {

    parser::convertor->printDebugInformation("Byte Code From Node Variable Declaration Created");

    byte_code::Byte_Code* _byte_code_memory_stack_load = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code_memory_stack_load) byte_code::Byte_Code(
        BYTE_CODE_MEMORY_STACK_LOAD,
        __variable_declaration_node->getByteSize()
    );

    parser::convertor->printDebugInformation("Byte Code From Node Variable Declaration End");

    return _byte_code_memory_stack_load;

}

void parser_helper::getByteCodeFromNodeFunctionDeclaration(parser::Ast_Node_Function_Declaration* __function_declaration_node) {

    parser::convertor->printDebugInformation("Byte Code From Node Function Declaration Created");

    // Parameters ?? TODO
    parser::convertor->setBlock(__function_declaration_node);

    parser::convertor->printDebugInformation("Byte Code From Node Function Declaration End");

}

utils::Linked_List <byte_code::Byte_Code*>* parser_helper::getByteCodeFromExpression(parser::Ast_Node_Expression* __expression_node) {

    parser::convertor->printDebugInformation("Byte Code for node Expression");

    utils::Linked_List <byte_code::Byte_Code*>* _rtr = new utils::Linked_List <byte_code::Byte_Code*>();
    utils::Linked_List <byte_code::Byte_Code*>* _;
    bool _c = true;

    while(__expression_node) {

        _ = getByteCodeFromExpressionS(__expression_node, _c);

        _rtr->join(_);

        _->destroy_content = 0;

        delete _;

    }

    return _rtr;

    parser::convertor->printDebugInformation("Byte Code for node Expression End");

}

utils::Linked_List <byte_code::Byte_Code*>* parser_helper::getByteCodeFromExpressionS(parser::Ast_Node_Expression*& __expression_node, bool& __c) {

    utils::Linked_List <byte_code::Byte_Code*>* _rtr = new utils::Linked_List <byte_code::Byte_Code*>();
    utils::Linked_List <byte_code::Byte_Code*>* _;
    int _frstOp, _frstOpPri;

    if (__c) {
        _ = getByteCodeFromNode(__expression_node->value);
        _rtr->join(
            _
        );
        _->destroy_content = 0;
        delete _;
    }
    else __c = true;

    _frstOp = __expression_node->operator_id;
    _frstOpPri = getExpressionPriority(_frstOp);

    if (__expression_node->expression) {

        __expression_node = (parser::Ast_Node_Expression*) __expression_node->expression;

        if (
            getExpressionPriority(__expression_node->operator_id) <= _frstOpPri
        ) {

            _ = getByteCodeFromExpressionS(__expression_node, __c);
            _rtr->join(
                _
            );
            _->destroy_content = 0;
            delete _;

            _rtr->add(
                getByteCodeOfExpressionId(
                    _frstOp
                )
            );

        } else {

            _ = getByteCodeFromNode(__expression_node->value);
            _rtr->join(
                _
            );
            _->destroy_content = 0;
            delete _;

            _rtr->add(
                getByteCodeOfExpressionId(
                    _frstOp
                )
            );

        }

    } else __expression_node = NULL;

    __c = false;

    return _rtr;

}

byte_code::Byte_Code* parser_helper::getByteCodeFromValue(parser::Ast_Node_Value* __value_node) {

    parser::convertor->printDebugInformation("Byte Code for node Value");

    byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code) byte_code::Byte_Code(
        BYTE_CODE_MEM_STACK_LOAD_IMPLICIT, __value_node->implicit_value_position
    );

    parser::convertor->printDebugInformation("Byte Code for node Value End");

    return _byte_code;

}

byte_code::Byte_Code* parser_helper::getByteCodeFromVariable(parser::Ast_Node_Variable* __variable_node) {

    parser::convertor->printDebugInformation("Byte Code for node Variable");

    byte_code::Byte_Code* _load_variable = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
    
    new (_load_variable) byte_code::Byte_Code(
        __variable_node->is_global ? BYTE_CODE_LOAD_VARIABLE_GLOBAL : BYTE_CODE_LOAD_VARIABLE,
        __variable_node->declaration_id
    );

    parser::convertor->printDebugInformation("Byte Code for node Variable End");

    return _load_variable;

}

byte_code::Byte_Code* parser_helper::getByteCodeFromFunctionCall(parser::Ast_Node_Function_Call* __function_call_node) {

    parser::convertor->printDebugInformation("Byte Code for node Function Call");

    // Parameters ?? TODO

    byte_code::Byte_Code* _function_call = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_function_call) byte_code::Byte_Code(
        BYTE_CODE_CALL, __function_call_node->declaration->body_position
    );

    parser::convertor->printDebugInformation("Byte Code for node Function Call End");

    return _function_call;

}

utils::Linked_List <byte_code::Byte_Code*>* parser_helper::getByteCodeFromPointerOperator(parser::Ast_Node_Pointer_Operator* __pointer_operator_node) {

    parser::convertor->printDebugInformation("Byte Code for node Pointer Operator");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_codes = new utils::Linked_List <byte_code::Byte_Code*>();

    utils::Linked_List <byte_code::Byte_Code*>* _call_byte_code = getByteCodeFromNode(__pointer_operator_node->value);

    _byte_codes->join(
        _call_byte_code
    );

    _call_byte_code->destroy_content = 0; delete _call_byte_code;

    byte_code::Byte_Code* _pointer_level = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_pointer_level) byte_code::Byte_Code(
        __pointer_operator_node->pointer_level > 0 ? BYTE_CODE_POINTER_LEVEL_UP : BYTE_CODE_POINTER_LEVEL_DOWN, 
        __pointer_operator_node->pointer_level > 0 ? __pointer_operator_node->pointer_level : __pointer_operator_node->pointer_level * -1
    );

    _byte_codes->add(_pointer_level);

    parser::convertor->printDebugInformation("Byte Code for node Pointer Operator End");

    return _byte_codes;

}

utils::Linked_List <byte_code::Byte_Code*>* parser_helper::getByteCodeFromParenthesis(parser::Ast_Node_Parenthesis* __parenthesis_node) {

    parser::convertor->printDebugInformation("Byte Code for node Parenthesis");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = getByteCodeFromExpression(
        __parenthesis_node->expression
    );

    parser::convertor->printDebugInformation("Byte Code for node Parenthesis End");

    return _byte_code;

}


