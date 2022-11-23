#include "./compiler_converter.h"

#include "./../byteCode/definitions.h" // Byte Code Definitions
#include "./../byteCode/byteCode.h" // Byte Code
#include "./../utils/linkedList.h" // Linked List
#include "./compiler_helper.h" // Compiler helper
#include "./parser_helper.h" // Parse helper
#include "./definitions.h" // Parser Definitions
#include "./compiler.h" // Compiler Control
#include "./ast_nodes.h" // Ast Nodes
#include "./ast.h" // Ast Storage
#include "./token.h" // Token

#include <iostream>

int parser_helper::getExpressionPriority(int _tkId) {

    if (_tkId >= TOKEN_MULTIPLICATION && _tkId <= TOKEN_MODULUS) return 2;
    if (_tkId >= TOKEN_ADDITION && _tkId <= TOKEN_SUBTRACTION) return 3;
    if (_tkId >= TOKEN_BITWISEAND && _tkId <= TOKEN_BITWISERIGHTSHIFT) return 4;
    if (_tkId >= TOKEN_AND && _tkId <= TOKEN_NOT) return 5;

    return 6;

}

byte_code::Byte_Code* parser_helper::getByteCodeOfExpressionId(int _tkId) {

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

utils::LinkedList <byte_code::Byte_Code*>* parser_helper::getByteCodeFromNode(parser::Ast_Node* _node, parser::Compiler_Control* _comCntrl) {

    // std::cout << "Node type -> " << _node->id << std::endl;

    switch (_node->id)
    {
    case AST_NODE_VALUE:

        return getByteCodeFromNodeValue(
            (parser::Ast_Node_Value*) _node, _comCntrl
        );

    case AST_NODE_EXPRESSION: 
    
        return getByteCodeFromNodeExpression(
            (parser::Ast_Node_Expression*) _node, _comCntrl
        );

    case AST_NODE_VARIABLE_DECLARATION:

        return getByteCodeFromNodeVariableDeclaration(
            (parser::Ast_Node_Variable_Declaration*) _node, _comCntrl
        );
    
    case AST_NODE_VARIABLE:

        return getByteCodeFromNodeVariable(
            (parser::Ast_Node_Variable*) _node, _comCntrl
        );

    case AST_NODE_VARIABLE_ASSIGNMENT:

        return getByteCodeFromNodeVariableAssign(
            (parser::Ast_Node_Variable_Assignment*) _node, _comCntrl
        );

    case AST_NODE_PARENTHESIS:

        return getByteCodeFromNodeParenthesis(
            (parser::Ast_Node_Parenthesis*) _node, _comCntrl
        );

    case AST_NODE_POINTER_OPERATORS:

        return getByteCodeFromNodePointerOperations(
            (parser::Ast_Node_Pointer_Operators*) _node, _comCntrl
        );

    case AST_NODE_FUNCTION_DECLARATION:

        getByteCodeFromNodeFunctionDeclaration(
            (parser::Ast_Node_Function_Declaration*) _node, _comCntrl
        );

        break;

    case AST_NODE_FUNCTION_CALL:

        return getByteCodeFromNodeFunctionCall(
            (parser::Ast_Node_Function_Call*) _node, _comCntrl
        );
    
    default:
        break;
    }

    return NULL;

}

utils::LinkedList <byte_code::Byte_Code*>* parser_helper::getByteCodeFromNodeValue(parser::Ast_Node_Value* _astValue, parser::Compiler_Control* _comCntrl) {

    utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
    byte_code::Byte_Code* _byteCode = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code)); 

    new (_byteCode) byte_code::Byte_Code(
        BYTECODE_MEM_STACK_LOAD_IMPLICIT, _astValue->valuePos
    );

    _rtr->add(
        _byteCode
    );

    return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* parser_helper::getByteCodeFromNodeVariable(parser::Ast_Node_Variable* _astVariable, parser::Compiler_Control* _comCntrl) {

    utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
    byte_code::Byte_Code* _loadVar = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    parser::Ast_Node_Variable_Declaration* _astVarDecl = NULL;
    int _varPos = 0;

    for (; _varPos < _comCntrl->varDecl->count; _varPos++)

        if (
            (*_comCntrl->varDecl)[_varPos]->declId == _astVariable->declId
        ) { std::cout << "Var founded" << std::endl; _astVarDecl = (*_comCntrl->varDecl)[_varPos]; break; }

    if (!_astVarDecl) { std::cout << "No variable declaration with given name" << std::endl; exit(-1); } // TODO

    new (_loadVar) byte_code::Byte_Code(
        BYTECODE_LOAD_VARIABLE,
        _astVariable->declId
    );

    _rtr->add(_loadVar);

    return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* parser_helper::getByteCodeFromNodeExpression(parser::Ast_Node_Expression* _astExpression, parser::Compiler_Control* _comCntrl) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
        bool _c = true;

        while(_astExpression) 
            _rtr->join(getByteCodeFromNodeExpressionS(_astExpression, _c, _comCntrl));

        return _rtr; 

}

utils::LinkedList <byte_code::Byte_Code*>* parser_helper::getByteCodeFromNodeExpressionS(parser::Ast_Node_Expression*& _astExpression, bool& _c, parser::Compiler_Control* _comCntrl) {

    utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
    int _frstOp, _frstOpPri;

    if (_c)
        _rtr->join(
            parser_helper::getByteCodeFromNode(_astExpression->frst, _comCntrl)
        );
    else _c = true;

    _frstOp = _astExpression->expId;
    _frstOpPri = getExpressionPriority(_frstOp);

    if (_astExpression->scnd) {

        _astExpression = (parser::Ast_Node_Expression*) _astExpression->scnd;

        if (
            getExpressionPriority(_astExpression->expId) <= _frstOpPri
        ) {

            _rtr->join(
                getByteCodeFromNodeExpressionS(_astExpression, _c, _comCntrl)
            );

            _rtr->add(
                parser_helper::getByteCodeOfExpressionId(
                    _frstOp
                )
            );

        } else {

            _rtr->join(
                parser_helper::getByteCodeFromNode(_astExpression->frst, _comCntrl)
            );

            _rtr->add(
                parser_helper::getByteCodeOfExpressionId(
                    _frstOp
                )
            );
        
        }

    } else _astExpression = NULL;

    _c = false;

    return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* parser_helper::getByteCodeFromNodeVariableDeclaration(
    parser::Ast_Node_Variable_Declaration* _astVarDecl, parser::Compiler_Control* _comCntrl) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
        byte_code::Byte_Code* _byteCode = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
        parser::Type_Information* _type = (*_comCntrl->ast_storage->types)[_astVarDecl->typePos];
        int _s = 0;

        _comCntrl->varDecl->add(
            _astVarDecl
        );

        if (_type->id == TOKEN_IDENTIFIER) { std::cout << "not done variable declaration 1" << std::endl; exit(-1); /* TODO */}

        else {

            _s = parser_helper::getSizePrimitiveType(_type->id);

        }

        new (_byteCode) byte_code::Byte_Code(BYTECODE_MEM_STACK_LOAD, _s);
        _rtr->add(_byteCode);

        return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNodeVariableAssign(parser::Ast_Node_Variable_Assignment* _astVarAssign, parser::Compiler_Control* _comCntrl) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();
        
        _rtr->join(
            getByteCodeFromNode(
                _astVarAssign->valueBeforeAssign, _comCntrl
            )
        );

        if (_astVarAssign->opIsLeft) {

            byte_code::Byte_Code* _bc = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

            new (_bc) byte_code::Byte_Code(
                (_astVarAssign->id == TOKEN_INCREMENT) ? BYTECODE_ASSIGN_INCREMENT_LEFT : BYTECODE_ASSIGN_DECREMENT_LEFT, 
                0
            );

            _rtr->add(
                _bc
            );

        } else {

            _rtr->join(
                getByteCodeFromNode(_astVarAssign->value, _comCntrl)
            );
        
            _rtr->add(
                getByteCodeOfExpressionId(
                    _astVarAssign->expId
                )
            );

        }

        return _rtr;

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNodeParenthesis(parser::Ast_Node_Parenthesis* _astParenthesis, parser::Compiler_Control* _comCntrl) {

        return getByteCodeFromNode(_astParenthesis->value, _comCntrl);

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNodePointerOperations(parser::Ast_Node_Pointer_Operators* _astPntrOprs, parser::Compiler_Control* _comCntrl) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();

        _rtr->join(
            getByteCodeFromNode(
                _astPntrOprs->value, _comCntrl
            )
        );

        for (int _ = _astPntrOprs->operations->count - 1; _ >= 0; _--)

            _rtr->add(
                getByteCodeOfExpressionId((*_astPntrOprs->operations)[_])
            );

        return _rtr;

}

void parser_helper::getByteCodeFromNodeFunctionDeclaration(parser::Ast_Node_Function_Declaration* _astFuncDecl, parser::Compiler_Control* _comCntrl) {

    std::cout << "Func decl decl id -> " << _astFuncDecl->declId << std::endl;

    _comCntrl->code_blocks->add(
        parser::Compiler_Code_Block::generate(
            _comCntrl, (parser::Ast_Node_Code_Block*) _astFuncDecl->body
        )
    );


    byte_code::Byte_Code* _paramsEnd = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_paramsEnd) byte_code::Byte_Code(
        BYTECODE_PARAMS_END,
        0
    );
    (*_comCntrl->code_blocks)[_comCntrl->code_blocks->count - 1]->byte_code->addFrst(_paramsEnd);

    // Add the parameters at first inverted
    for (int _ = 0; _ < _astFuncDecl->parameters->count; _++)

        (*_comCntrl->code_blocks)[_comCntrl->code_blocks->count - 1]->byte_code->addFrst(
            getByteCodeFromNodeVariableDeclaration( // Memory leak
                (parser::Ast_Node_Variable_Declaration*) (*_astFuncDecl->parameters)[_], _comCntrl
            )->frst->object
        );

    _comCntrl->funcDecl->add(_astFuncDecl);

}

utils::LinkedList <byte_code::Byte_Code*>* 
    parser_helper::getByteCodeFromNodeFunctionCall(parser::Ast_Node_Function_Call* _astFuncCall, parser::Compiler_Control* _comCntrl) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();

        parser::Ast_Node_Function_Declaration* _astFuncDecl = NULL;
        int _funcPos = 0;

        std::cout << "Func call decl id -> " << _astFuncCall->declId << std::endl;

        for (; _funcPos < _comCntrl->funcDecl->count; _funcPos++)

            if (
                (*_comCntrl->funcDecl)[_funcPos]->declId == _astFuncCall->declId
            ) { _astFuncDecl = (*_comCntrl->funcDecl)[_funcPos]; break; }

        if (!_astFuncDecl) { std::cout << "No function declaration with given name" << std::endl; exit(-1); } // TODO

        if (!parser_helper::confirmFunctionParameters(_astFuncDecl->parameters, _astFuncCall->parameters, _comCntrl)) {

            std::cout << "No same lenght of inputs in function declaration and function call" << std::endl;
            std::cout << "Or No same type size of inputs in function declaration and function call" << std::endl;

            exit(-1);

        }

        for (int _ = 0; _ < _astFuncCall->parameters->count; _++) {

            _rtr->join(
                getByteCodeFromNode(
                    (*_astFuncCall->parameters)[_], _comCntrl
                )
            );

            byte_code::Byte_Code* _assign = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

            new (_assign) byte_code::Byte_Code(
                BYTECODE_ASSIGN,
                0
            );
            _rtr->add(_assign);

        }

        byte_code::Byte_Code* _tkFuncCall = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_tkFuncCall) byte_code::Byte_Code(
            BYTECODE_FUNCTION_CALL,
            _funcPos
        );
        _rtr->add(_tkFuncCall);

        return _rtr;

}


