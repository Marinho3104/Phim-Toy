#include "./compiler_byte_code_converter.h"

#include "./parser_definitions.h"
#include "./compiler.h"
#include "./../byteCode/byteCode.h"
#include "./../byteCode/byte_code_definitions.h"
#include "./../utils/linkedList.h"
#include "./token.h"

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
    case TOKEN_ADDITION:
        
        new(_) byte_code::Byte_Code(BYTECODE_ADDITION, 0);

        break;
    case TOKEN_SUBTRACTION:
        
        new(_) byte_code::Byte_Code(BYTECODE_SUBTRACTION, 0);

        break;
    case TOKEN_MULTIPLICATION:
        
        new(_) byte_code::Byte_Code(BYTECODE_MULTIPLICATION, 0);

        break;
    case TOKEN_DIVISION:
        
        new(_) byte_code::Byte_Code(BYTECODE_DIVISION, 0);

        break;
    case TOKEN_MODULUS:
        
        new(_) byte_code::Byte_Code(BYTECODE_MODULOS, 0);

        break;   
    
    case TOKEN_ADDITIONASSIGMENT:
        
        new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_ADDITION, 0);

        break;
    case TOKEN_SUBTRACTIONASSIGMENT:
        
        new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_SUBTRACTION, 0);

        break;
    case TOKEN_MULTIPLICATIONASSIGMENT:
        
        new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_MULTIPLICATION, 0);

        break;
    case TOKEN_DIVISIONASSIGMENT:
        
        new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_DIVISION, 0);

        break;
    case TOKEN_MODULOSASSIGMENT:
        
        new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_MODULOS, 0);

        break; 
    case TOKEN_INCREMENT:

        new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_INCREMENT, 0);

        break;

    case TOKEN_DECREMENT:

        new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_DECREMENT, 0);

        break;

    case TOKEN_EQUAL:

        new (_) byte_code::Byte_Code(BYTECODE_ASSIGN, 0);

        break;

    case TOKEN_POINTER:

        new (_) byte_code::Byte_Code(BYTECODE_POINTER_OPERATION, 0);

        break;

    case TOKEN_ADDRESS:

        new (_) byte_code::Byte_Code(BYTECODE_ADDRESS_OPERATION, 0);

        break;

    }

    return _;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNode(Ast_Node* __astNode, Compiler_Control* __comCntrl) {

    // std::cout << "Node type -> " << __astNode->node_id << std::endl;
    utils::LinkedList <byte_code::Byte_Code*>* _;

    switch (__astNode->node_id)
    {
    case AST_NODE_EXPRESSION:
        _ = getByteCodeFromNodeExpression(
            (Ast_Node_Expression*) __astNode, __comCntrl
        );
        break;

    case AST_NODE_VALUE: 
        _ = getByteCodeFromNodeValue(
            (Ast_Node_Value*) __astNode, __comCntrl
        ); break;

    case AST_NODE_VARIABLE_DECLARATION:

        _ = getByteCodeFromNodeVariableDeclaration(
            (Ast_Node_Variable_Declaration*) __astNode, __comCntrl
        ); break;

    case AST_NODE_VARIABLE:

        _ = getByteCodeFromNodeVariable(
            (Ast_Node_Variable*) __astNode, __comCntrl
        ); break;

    case AST_NODE_VARIABLE_ASSIGNMENT:

        _ = getByteCodeFromNodeVariableAssign(
            (Ast_Node_Variable_Assignment*) __astNode, __comCntrl
        ); break;

    case AST_NODE_PARENTHESIS:

        _ = getByteCodeFromNodeParenthesis(
            (Ast_Node_Parenthesis*) __astNode, __comCntrl
        ); break;

    case AST_NODE_CODE_BLOCK:

        _ = getByteCodeFromNodeCodeBlock(
            (Ast_Node_Code_Block*) __astNode, __comCntrl
        ); break;

    default: new Compiler_Exception("Unexpected Node type");
    }

    return _;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNodeExpression(parser::Ast_Node_Expression* __astExp, parser::Compiler_Control* __comCntrl) {

        __comCntrl->printDebugInfo("--> Byte Code for node expression <--");
        
        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
        utils::LinkedList <byte_code::Byte_Code*>* _;
        bool _c = true;

        while(__astExp) {
            _ = getByteCodeFromNodeExpressionS(__astExp, _c, __comCntrl);
        
            _rtr->join(_);

            _->destroy_content = 0;

            delete _;

        }

        return _rtr; 

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNodeExpressionS(parser::Ast_Node_Expression*& _astExpression, bool& _c, parser::Compiler_Control* _comCntrl) {

    utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
    utils::LinkedList <byte_code::Byte_Code*>* _;
    int _frstOp, _frstOpPri;

    if (_c) {
        _ = parser::getByteCodeFromNode(_astExpression->first, _comCntrl);
        _rtr->join(
            _
        );
        _->destroy_content = 0;
        delete _;
    }
    else _c = true;

    _frstOp = _astExpression->expression_id;
    _frstOpPri = getExpressionPriority(_frstOp);

    if (_astExpression->second) {

        _astExpression = (parser::Ast_Node_Expression*) _astExpression->second;

        if (
            getExpressionPriority(_astExpression->expression_id) <= _frstOpPri
        ) {

            _ = parser::getByteCodeFromNodeExpressionS(_astExpression, _c, _comCntrl);
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

            _ = parser::getByteCodeFromNode(_astExpression->first, _comCntrl);
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

    } else _astExpression = NULL;

    _c = false;

    return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNodeValue(parser::Ast_Node_Value* __astVal, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node value <--");

    utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
    byte_code::Byte_Code* _byteCode = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code)); 

    new (_byteCode) byte_code::Byte_Code(
        BYTECODE_MEM_STACK_LOAD_IMPLICIT, __astVal->value_position
    );

    _rtr->add(
        _byteCode
    );

    return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNodeVariableDeclaration(
    parser::Ast_Node_Variable_Declaration* __astVarDecl, parser::Compiler_Control* __comCntrl) {

        __comCntrl->printDebugInfo("--> Byte Code for node variable declaration <--");

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
        byte_code::Byte_Code* _byteCode = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
        int _s = __astVarDecl->getByteSize();

        new (_byteCode) byte_code::Byte_Code(BYTECODE_MEM_STACK_LOAD, _s);
        _rtr->add(_byteCode);

        if (__comCntrl->current_compiler_code_block->compiler_declarations->isDeclared(__astVarDecl->declaration_id))
            new Compiler_Exception("Redeclaration of name");

        __comCntrl->current_compiler_code_block->compiler_declarations->variable_declaration->add(__astVarDecl);

        return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNodeVariable(parser::Ast_Node_Variable* __astVar, parser::Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node variable <--");

    utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
    byte_code::Byte_Code* _loadVar = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    parser::Ast_Node_Variable_Declaration* _astVarDecl = __comCntrl->current_compiler_code_block->compiler_declarations->getVariableDeclaration(__astVar->declaration_id);
    parser::Ast_Node_Variable_Declaration* _astVarDeclPrev = 
        __comCntrl->current_compiler_code_block->previous_compiler_declarations ? __comCntrl->current_compiler_code_block->previous_compiler_declarations->getVariableDeclaration(__astVar->declaration_id) : NULL;
    if (!_astVarDecl && !_astVarDeclPrev) new Compiler_Exception("No variable in declared with given name");

    __astVar->variable_declaration = _astVarDecl;

    new (_loadVar) byte_code::Byte_Code(
        BYTECODE_LOAD_VARIABLE,
        __astVar->declaration_id
    );

    _rtr->add(_loadVar);

    return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser::getByteCodeFromNodeVariableAssign(parser::Ast_Node_Variable_Assignment* __astVarAssign, parser::Compiler_Control* __comCntrl) {

        __comCntrl->printDebugInfo("--> Byte Code for node variable assign <--");

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>(), *_;
        
        _ = getByteCodeFromNode(
            __astVarAssign->value_before_assign, __comCntrl
        );

        _rtr->join(
            _
        );

        _->destroy_content = 0; delete _;

        if (__astVarAssign->operation_is_left) {
            
            byte_code::Byte_Code* _bc = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

            new (_bc) byte_code::Byte_Code(
                (__astVarAssign->expression_id == TOKEN_INCREMENT) ? BYTECODE_ASSIGN_INCREMENT_LEFT : BYTECODE_ASSIGN_DECREMENT_LEFT, 
                0
            );

            _rtr->add(
                _bc
            );

        } else {

            _ = getByteCodeFromNode(__astVarAssign->value, __comCntrl);

            _rtr->join(
                _
            );
        
            _->destroy_content = 0; delete _;

            _rtr->add(
                getByteCodeOfExpressionId(
                    __astVarAssign->expression_id
                )
            );

        }

        return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNodeParenthesis(parser::Ast_Node_Parenthesis* __astPar, parser::Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node parenthesis <--");

    return getByteCodeFromNode(__astPar->value, __comCntrl);

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNodeCodeBlock(parser::Ast_Node_Code_Block* __astCodeBlock, parser::Compiler_Control* __comCntrl) {

    utils::LinkedList <byte_code::Byte_Code*>* _ = new utils::LinkedList <byte_code::Byte_Code*>();

    __comCntrl->printDebugInfo("--> Byte Code for node code block <--");

    parser::Compiler_Code_Block::generate(
        __comCntrl, __astCodeBlock
    );

    if (__astCodeBlock->environment_id == AST_NODE_CODE_BLOCK_ENVIRONMENT_NORMAL) {

        byte_code::Byte_Code* _bc = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_bc) byte_code::Byte_Code(
            BYTECODE_CALL, __comCntrl->compiled_code_blocks->count - 1
        );

        _->add(
            _bc
        );

    }

    __comCntrl->printDebugInfo("--> Byte Code for node code block end <--");

    return _;

}


