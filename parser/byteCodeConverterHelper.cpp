#include "./byteCodeConverterHelper.h"

#include "./../byteCode/definitions.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./byteCodeConverter.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

int parser_helper::getExpressionPriority(int _expressionPos, utils::LinkedList<int>* _expIDTable) {

    if (_expressionPos == -1) return 6;
    
    int _exprValue = (*_expIDTable)[_expressionPos];

    if (_exprValue >= TOKEN_MULTIPLICATION && _exprValue <= TOKEN_MODULUS) return 2;
    if (_exprValue >= TOKEN_ADDITION && _exprValue <= TOKEN_SUBTRACTION) return 3;
    if (_exprValue >= TOKEN_BITWISEAND && _exprValue <= TOKEN_BITWISERIGHTSHIFT) return 4;
    if (_exprValue >= TOKEN_AND && _exprValue <= TOKEN_NOT) return 5;

}

byte_code::Byte_Code* parser_helper::getByteCodeOfExpressionId(int _expressionId) {

    byte_code::Byte_Code* _ = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    switch (_expressionId)
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
    default: return NULL;
    }

    return _;

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNode(parser::Ast_Node* _crrntNode, parser::Byte_Code_Converter_Control* _bcCnvrCntrl) {

        switch (_crrntNode->type)
        {
        case AST_NODE_VALUE: return parser_helper::getByteCodeFromNodeValue((parser::Ast_Node_Value*) _crrntNode);
        case AST_NODE_EXPRESSION: return parser_helper::getByteCodeFromNodeExpressison((parser::Ast_Node_Expression*) _crrntNode, _bcCnvrCntrl);
        case AST_NODE_PARENTHESIS: return parser_helper::getByteCodeFromNodeParenthesis((parser::Ast_Node_Parenthesis*) _crrntNode, _bcCnvrCntrl);
        case AST_NODE_VARIABLE_DECLARATION: return parser_helper::getByteCodeFromNodeVariableDeclaration((parser::Ast_Node_Variable_Declaration*) _crrntNode, _bcCnvrCntrl);
        case AST_NODE_VARIABLE: return parser_helper::getByteCodeFromNodeVariable((parser::Ast_Node_Variable*) _crrntNode);

        default:
            break;
        }

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNodeValue(parser::Ast_Node_Value* _value) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();

        _rtr->add(
            new byte_code::Byte_Code(
                BYTECODE_MEM_STACK_LOAD_CONST,
                _value->valuePos
            )
        );

        return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNodeVariable(parser::Ast_Node_Variable* _variable) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();

        _rtr->add(
            new byte_code::Byte_Code(
                BYTECODE_LOAD_VARIABLE,
                _variable->namePos
            )
        );

        return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNodeExpressison(parser::Ast_Node_Expression* _expression, parser::Byte_Code_Converter_Control* _bcCnvrCntrl) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
        bool _c = true;

        while(_expression) 
            _rtr->join(getByteCodeFromNodeExpressisonS(&_expression, _bcCnvrCntrl, _c));

        return _rtr;        

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNodeExpressisonS(parser::Ast_Node_Expression** _expression, parser::Byte_Code_Converter_Control* _bcCnvrCntrl, bool& _c) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
        int _frstOp, _frstOpPri;

        if (_c)
            _rtr->join(
                parser_helper::getByteCodeFromNode((*_expression)->frst, _bcCnvrCntrl)
            );
        else _c = true;

        _frstOp = (*_expression)->expressionIdPos;
        _frstOpPri = getExpressionPriority(_frstOp, _bcCnvrCntrl->storage->expressionsId);

        if ((*_expression)->scnd) {

            (*_expression) = (parser::Ast_Node_Expression*) (*_expression)->scnd;

            if (
                getExpressionPriority((*_expression)->expressionIdPos, _bcCnvrCntrl->storage->expressionsId) <= _frstOpPri
            ) {

                _rtr->join(
                    getByteCodeFromNodeExpressisonS(_expression, _bcCnvrCntrl, _c)
                );

                _rtr->add(
                    parser_helper::getByteCodeOfExpressionId(
                        (*_bcCnvrCntrl->storage->expressionsId)[_frstOp]
                    )
                );

            } else {

                _rtr->join(
                    parser_helper::getByteCodeFromNode((*_expression)->frst, _bcCnvrCntrl)
                );

                _rtr->add(
                    parser_helper::getByteCodeOfExpressionId(
                        (*_bcCnvrCntrl->storage->expressionsId)[_frstOp]
                    )
                );
            
            }

        } else (*_expression) = NULL;

        _c = false;

        return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNodeParenthesis(parser::Ast_Node_Parenthesis* _parenthesis, parser::Byte_Code_Converter_Control* _bcCnvrCntrl) {

        return getByteCodeFromNode(_parenthesis->value, _bcCnvrCntrl);

}

utils::LinkedList <byte_code::Byte_Code*>* parser_helper::getByteCodeFromNodeVariableDeclaration(
    parser::Ast_Node_Variable_Declaration* _varDeclaration, parser::Byte_Code_Converter_Control* _bcCnvrCntrl) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
        byte_code::Byte_Code* _loadName = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
        byte_code::Byte_Code* _memStackLoad = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
        byte_code::Byte_Code* _value = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_loadName) byte_code::Byte_Code(BYTECODE_LOAD_NAME, _varDeclaration->namePos);
        _rtr->add(_loadName);

        new (_memStackLoad) byte_code::Byte_Code(BYTECODE_MEM_STACK_LOAD, _varDeclaration->typePos);
        _rtr->add(_memStackLoad);

        if (_varDeclaration->value) {

            _rtr->join(
                getByteCodeFromNode(_varDeclaration->value, _bcCnvrCntrl)
            );

            new(_value) byte_code::Byte_Code(BYTECODE_ASSIGN, 0);

            _rtr->add(_value);

        } else free(_value);

        return _rtr;

}


