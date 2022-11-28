#include "./compiler_byte_code_converter.h"

#include "./parser_definitions.h"
#include "./compiler.h"
#include "./../byteCode/byteCode.h"
#include "./../byteCode/byte_code_definitions.h"
#include "./../utils/linkedList.h"
#include "./token.h"
#include "./ast.h"
#include "./compiler_helper.h"

#include <iostream>

parser::Type_Information parser::getTypeOfNode(Ast_Node* __astNode) {

    parser::Type_Information _, *__;

    switch (__astNode->node_id)
    {
    case AST_NODE_VALUE: // Needs to be free

        __ = ((Ast_Node_Value*) __astNode)->getType();

        _.token_id = __->token_id;
        _.pointer_level = __->pointer_level;
        _.reference_level = __->reference_level;

        __->~Type_Information(); free(__);

        goto rtr;

    case AST_NODE_VARIABLE:

        __ = ((Ast_Node_Variable*) __astNode)->getType();

        if (!__) break;

        _ = *__;

        goto rtr;

    case AST_NODE_PARENTHESIS:

        __ = ((Ast_Node_Parenthesis*) __astNode)->getType();

        if (!__) break;

        _ = *__;

        goto rtr;

    default: break;
    }

    new Compiler_Exception("Unexpected Node to get type");

rtr:

    return _;

}

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
    utils::LinkedList <byte_code::Byte_Code*>* _ = NULL;

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

    case AST_NODE_POINTER_OPERATORS:

        _ = getByteCodeFromNodePointerOperators(
            (Ast_Node_Pointer_Operators*) __astNode, __comCntrl
        ); break;

    case AST_NODE_FUNCTION_DECLARATION:

        getByteCodeFromNodeFunctionDeclaration(
            (Ast_Node_Function_Declaration*) __astNode, __comCntrl
        ); break;

    case AST_NODE_FUNCTION_CALL:

        _ = getByteCodeFromNodeFunctionCall(
            (Ast_Node_Function_Call*) __astNode, __comCntrl
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

    parser::Ast_Node_Variable_Declaration* _astVarDecl = __comCntrl->current_compiler_code_block->getVariableDeclaration(__astVar->declaration_id);

    if (!_astVarDecl) new Compiler_Exception("No variable in declared with given name");

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
            BYTECODE_CALL_LOCAL, __comCntrl->compiled_code_blocks->count - 1
        );

        _->add(
            _bc
        );

    }

    __comCntrl->printDebugInfo("--> Byte Code for node code block end <--");

    return _;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNodePointerOperators(
    parser::Ast_Node_Pointer_Operators* __astPtnrOprs, parser::Compiler_Control* __comCntrl) {

        __comCntrl->printDebugInfo("--> Byte Code for node pointer operations <--");

        utils::LinkedList <byte_code::Byte_Code*>* _ = new utils::LinkedList <byte_code::Byte_Code*>(), *__t;
        byte_code::Byte_Code* _bc = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
        int _off, _bcId, _lvl;

        __t = getByteCodeFromNode(__astPtnrOprs->value, __comCntrl);

        _->join(__t);

        __t->destroy_content = 0; delete __t;

        parser::Type_Information _valueType = getTypeOfNode(__astPtnrOprs->value);

        if ((_off = _valueType.pointer_level - __astPtnrOprs->pntrLvl) < 0) new Ast_Exception("Error pointer operators");

        _bcId = _off > _valueType.pointer_level ? BYTECODE_POINTER_UP : BYTECODE_POINTER_DOWN;
        _lvl = _bcId == BYTECODE_POINTER_UP ? _off - _valueType.pointer_level : _valueType.pointer_level - _off;

        new (_bc) byte_code::Byte_Code(
            _bcId, _lvl
        );

        _->add(_bc);

        return _;

}

void parser::getByteCodeFromNodeFunctionDeclaration(parser::Ast_Node_Function_Declaration* __astFuncDecl, parser::Compiler_Control* __comCntrl) {

        __comCntrl->printDebugInfo("--> Byte Code for node function declaration <--");

        if (__comCntrl->current_compiler_code_block->compiler_declarations->isDeclared(__astFuncDecl->declaration_id)) {

            parser::Ast_Node_Function_Declaration* _ = __comCntrl->current_compiler_code_block->getFunctionDeclaration(__astFuncDecl->declaration_id);

            if (!_) new Compiler_Exception("Redeclaration of name");
            if (_->body) new Compiler_Exception("Redefenition of function body");

            Ast_Node_Code_Block* _cpyBody = __astFuncDecl->body;

            __astFuncDecl = _;

            __astFuncDecl->body = _cpyBody;

        } else __comCntrl->current_compiler_code_block->compiler_declarations->functions_declaraction->add(__astFuncDecl);

        if (!__astFuncDecl->body) { __astFuncDecl->body_pos = -1; return; }

        utils::LinkedList <byte_code::Byte_Code*>* _t;

        parser::Compiler_Code_Block::generate(
            __comCntrl, __astFuncDecl->body
        );

        byte_code::Byte_Code* _bcNN = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_bcNN) byte_code::Byte_Code(
            BYTECODE_NN,
            0
        );

        __astFuncDecl->body_pos = __comCntrl->compiled_code_blocks->count - 1;

        (*__comCntrl->compiled_code_blocks)[__comCntrl->compiled_code_blocks->count - 1]->byte_code->addFrst(_bcNN);

        (*__comCntrl->compiled_code_blocks)[__comCntrl->compiled_code_blocks->count - 1]->addParameters(__comCntrl, __astFuncDecl->parameters);

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNodeFunctionCall(parser::Ast_Node_Function_Call* __astFuncCall, parser::Compiler_Control* __comCntrl) {

        utils::LinkedList <byte_code::Byte_Code*>* _rtr = new utils::LinkedList <byte_code::Byte_Code*>();

        parser::Ast_Node_Function_Declaration* _astFuncDecl = __comCntrl->current_compiler_code_block->getFunctionDeclaration(__astFuncCall->declaration_id);
        if (!_astFuncDecl) new Compiler_Exception("No function declaration with given name");

        if (!parser_helper::confirmFunctionParameters(_astFuncDecl->parameters, __astFuncCall->parameters)) 
            new Compiler_Exception(
                "No same lenght of inputs in function declaration and function call\n" \
                "Or No same type size of inputs in function declaration and function call"
            );

        byte_code::Byte_Code* _tkFuncCall = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        if (!_astFuncDecl->body) {__comCntrl->forward_declarations_byte_code->add(_tkFuncCall); __comCntrl->forward_declarations_declaration->add(_astFuncDecl); }

        new (_tkFuncCall) byte_code::Byte_Code(
            BYTECODE_CALL,
            _astFuncDecl->body_pos
        );
        _rtr->add(_tkFuncCall);

        return _rtr;

}

void parser::getByteCodeFromNodeStructDeclaration(parser::Ast_Node_Struct_Declaration* __astStrctDecl, parser::Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node struct declaration <--");

    if (__comCntrl->current_compiler_code_block->compiler_declarations->isDeclared(__astStrctDecl->declaration_id)) {

        parser::Ast_Node_Struct_Declaration* _ = __comCntrl->current_compiler_code_block->getStructDeclaration(__astStrctDecl->declaration_id);

        if (!_) new Compiler_Exception("Redeclaration of name");

        if (_->functions || _->fields) new Compiler_Exception("Redefenition of struct body");

        utils::LinkedList <Ast_Node_Function_Declaration*>* _cpyFuncDecl = _->functions;
        utils::LinkedList <Ast_Node*>* _cpyFields = _->fields;

        __astStrctDecl = _;

        __astStrctDecl->functions = _cpyFuncDecl;
        __astStrctDecl->fields = _cpyFields;

    } else __comCntrl->current_compiler_code_block->compiler_declarations->structs_declaration->add(__astStrctDecl);

    /* TODO
    
    if (__comCntrl->current_compiler_code_block->compiler_declarations->isDeclared(__astStrctDecl->declaration_id)) {

        parser::Ast_Node_Struct_Declaration* _ = __comCntrl->current_compiler_code_block->getStructDeclaration(__astStrctDecl->declaration_id);

        if (!_) new Compiler_Exception("Redeclaration of name");

        if (_->functions || _->fields) new Compiler_Exception("Redefenition of struct body");

        utils::LinkedList <Ast_Node_Function_Declaration*>* _cpyFuncDecl = _->functions;
        utils::LinkedList <Ast_Node*>* _cpyFields = _->fields;

        __astStrctDecl = _;

        __astStrctDecl->functions = _cpyFuncDecl;
        __astStrctDecl->fields = _cpyFields;

    } else __comCntrl->current_compiler_code_block->compiler_declarations->structs_declaration->add(__astStrctDecl);

    if (!__astStrctDecl->functions || !__astStrctDecl->fields) { __astStrctDecl->body_pos = -1; return; }
    */

}



