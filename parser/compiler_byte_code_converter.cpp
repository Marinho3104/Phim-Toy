#include "./compiler_byte_code_converter.h"

#include "./../byteCode/byte_code_definitions.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./compiler.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>


int parser::getExpressionPriority(int _tkId) {

    if (_tkId >= TOKEN_MULTIPLICATION && _tkId <= TOKEN_MODULUS) return 2;
    if (_tkId >= TOKEN_ADDITION && _tkId <= TOKEN_SUBTRACTION) return 3;
    if (_tkId >= TOKEN_BITWISEAND && _tkId <= TOKEN_BITWISERIGHTSHIFT) return 4;
    if (_tkId >= TOKEN_AND && _tkId <= TOKEN_NOT) return 5;

    return 6;

}

byte_code::Byte_Code* parser::getByteCodeOfExpressionId(int _tkId) {

    byte_code::Byte_Code* _ = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    switch (_tkId)
    {
    case TOKEN_ADDITION: new(_) byte_code::Byte_Code(BYTECODE_ADDITION, 0); break;
    case TOKEN_SUBTRACTION: new(_) byte_code::Byte_Code(BYTECODE_SUBTRACTION, 0); break;
    default: new Compiler_Exception("Expression not supported");
    }

    return _;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNode(Ast_Node* __node, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    utils::LinkedList <byte_code::Byte_Code*>* _ = new utils::LinkedList <byte_code::Byte_Code*>();

    // std::cout << "Node -> " << __node->node_id << std::endl;

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

        case AST_NODE_STRUCT_DECLARATION:

            parser::getByteCodeFromStructDeclaration(
                (Ast_Node_Struct_Declaration*) __node,
                __crrnt, __comCntrl
            ); break;

        case AST_NODE_EXPRESSION:

            delete _;

            _ = parser::getByteCodeFromExpression(
                (Ast_Node_Expression*) __node,
                __crrnt, __comCntrl
            ); break;

        case AST_NODE_VALUE:

            _->add(
                parser::getByteCodeFromValue(
                    (Ast_Node_Value*) __node,
                    __crrnt, __comCntrl
                )
            ); break;

        case AST_NODE_VARIABLE:

            _->add(
                parser::getByteCodeFromVariable(
                    (Ast_Node_Variable*) __node,
                    __crrnt, __comCntrl
                )
            ); break;

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

void parser::getByteCodeFromStructDeclaration(Ast_Node_Struct_Declaration* __strcDecl, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for Struct Declaration <--");

    __strcDecl->body_pos = Compiler_Code_Block::generate(__comCntrl, __strcDecl, __crrnt);

    __crrnt->compiler_declarations->structs_declarations->add(__strcDecl);

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromExpression(Ast_Node_Expression* __exp, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node Expression <--");

    utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
    utils::LinkedList <byte_code::Byte_Code*>* _;
    bool _c = true;

    while(__exp) {
        _ = getByteCodeFromExpressionS(__exp, __crrnt, __comCntrl, _c);

        _rtr->join(_);

        _->destroy_content = 0;

        delete _;

    }

    return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromExpressionS(Ast_Node_Expression*& __exp, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl, bool& _c) {

    utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
    utils::LinkedList <byte_code::Byte_Code*>* _;
    int _frstOp, _frstOpPri;

    if (_c) {
        _ = parser::getByteCodeFromNode(__exp->first, __crrnt, __comCntrl);
        _rtr->join(
            _
        );
        _->destroy_content = 0;
        delete _;
    }
    else _c = true;

    _frstOp = __exp->expression_id;
    _frstOpPri = getExpressionPriority(_frstOp);

    if (__exp->second) {

        __exp = (parser::Ast_Node_Expression*) __exp->second;

        if (
            getExpressionPriority(__exp->expression_id) <= _frstOpPri
        ) {

            _ = parser::getByteCodeFromExpressionS(__exp, __crrnt, __comCntrl, _c);
            _rtr->join(
                _
            );
            _->destroy_content = 0;
            delete _;

            _rtr->add(
                parser::getByteCodeOfExpressionId(
                    _frstOp
                )
            );

        } else {

            _ = parser::getByteCodeFromNode(__exp->first, __crrnt, __comCntrl);
            _rtr->join(
                _
            );
            _->destroy_content = 0;
            delete _;

            _rtr->add(
                parser::getByteCodeOfExpressionId(
                    _frstOp
                )
            );

        }

    } else __exp = NULL;

    _c = false;

    return _rtr;

}

byte_code::Byte_Code* parser::getByteCodeFromValue(Ast_Node_Value* __value, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node Value <--");

    byte_code::Byte_Code* _byteCode = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byteCode) byte_code::Byte_Code(
        BYTECODE_MEM_STACK_LOAD_IMPLICIT, __value->value_position
    );

    return _byteCode;
}

byte_code::Byte_Code* parser::getByteCodeFromVariable(Ast_Node_Variable* __var, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node Variable <--");

    byte_code::Byte_Code* _loadVar = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    parser::Ast_Node_Variable_Declaration* _astVarDecl = __crrnt->getVariableDeclaration(__var->declaration_id);

    if (!_astVarDecl) new Compiler_Exception("No variable in declared with given name");

    __var->variable_declaration = _astVarDecl;

    new (_loadVar) byte_code::Byte_Code(
        BYTECODE_LOAD_VARIABLE,
        __var->declaration_id
    );

    return _loadVar;

}


