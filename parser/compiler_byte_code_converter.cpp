#include "./compiler_byte_code_converter.h"

#include "./../byteCode/byte_code_definitions.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./compiler_helper.h"
#include "./compiler.h"
#include "./token.h"

#include <iostream>

parser::Type_Information parser::getTypeInformationOfNode(Ast_Node* __node) {

    Type_Information* _temp, _typeInfo;

    switch (__node->node_id)
    {
    case AST_NODE_VALUE: // Need to be free

        _temp = ((Ast_Node_Value*) __node)->getType();

        _typeInfo = *_temp;

        delete _temp;

        goto rtr;

    case AST_NODE_VARIABLE:

        _temp = ((Ast_Node_Variable*) __node)->getType();

        if (!_temp) break;

        _typeInfo = *_temp;

        goto rtr;
    
    default: break;
    }

    new Compiler_Exception("Unexpected node to get type from");

rtr:

    return _typeInfo;

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
        case TOKEN_ADDITION: new(_) byte_code::Byte_Code(BYTECODE_ADDITION, 0); break;
        case TOKEN_SUBTRACTION: new(_) byte_code::Byte_Code(BYTECODE_SUBTRACTION, 0); break;
        case TOKEN_MULTIPLICATION: new(_) byte_code::Byte_Code(BYTECODE_MULTIPLICATION, 0); break;
        case TOKEN_DIVISION: new(_) byte_code::Byte_Code(BYTECODE_DIVISION, 0); break;
        case TOKEN_MODULUS: new(_) byte_code::Byte_Code(BYTECODE_MODULOS, 0); break;
        case TOKEN_ADDITIONASSIGMENT: new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_ADDITION, 0); break;
        case TOKEN_SUBTRACTIONASSIGMENT: new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_SUBTRACTION, 0); break;
        case TOKEN_MULTIPLICATIONASSIGMENT: new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_MULTIPLICATION, 0); break;
        case TOKEN_DIVISIONASSIGMENT: new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_DIVISION, 0); break;
        case TOKEN_MODULOSASSIGMENT: new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_MODULOS, 0); break;
        case TOKEN_INCREMENT: new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_INCREMENT, 0); break;
        case TOKEN_DECREMENT: new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_DECREMENT, 0); break;
        case TOKEN_INCREMENT_LEFT: new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_INCREMENT_LEFT, 0); break;
        case TOKEN_DECREMENT_LEFT: new(_) byte_code::Byte_Code(BYTECODE_ASSIGN_DECREMENT_LEFT, 0); break;
        case TOKEN_EQUAL: new (_) byte_code::Byte_Code(BYTECODE_ASSIGN, 0); break;
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

        case AST_NODE_VARIABLE_ASSIGNMENT:

            delete _;

            _ = parser::getByteCodeFromVariableAssignment(
                (Ast_Node_Variable_Assignment*) __node,
                __crrnt, __comCntrl
            ); break;

        case AST_NODE_POINTER_OPERATORS:

            delete _;

            _ = parser::getByteCodeFromPointerOperators(
                (Ast_Node_Pointer_Operators*) __node,
                __crrnt, __comCntrl
            ); break;

        case AST_NODE_PARENTHESIS:

            delete _;

            _ = parser::getByteCodeFromParenthesis(
                (Ast_Node_Parenthesis*) __node,
                __crrnt, __comCntrl
            ); break;

        case AST_NODE_FUNCTION_CALL:

            delete _;

            _ = parser::getByteCodeFromFunctionCall(
                (Ast_Node_Function_Call*) __node,
                __crrnt, __comCntrl
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

    parser::Ast_Node_Variable_Declaration* _astVarDecl = NULL;
    
    if (__var->name_space) {

        __crrnt = __comCntrl->getCodeBlockFromNameSpace(__var->name_space);

        if (!__crrnt) new Compiler_Exception("No variable is declared with given name - name space");

    }

    _astVarDecl = __crrnt->getVariableDeclaration(__var->declaration_id);

    if (!_astVarDecl) new Compiler_Exception("No variable is declared with given name");

    __var->variable_declaration = _astVarDecl;

    new (_loadVar) byte_code::Byte_Code(
        __var->is_global ? BYTECODE_LOAD_VARIABLE_GLOBAL : BYTECODE_LOAD_VARIABLE,
        __var->declaration_id
    );

    return _loadVar;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromVariableAssignment(Ast_Node_Variable_Assignment* __varAssign, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node Value Assignment <--");

    utils::LinkedList <byte_code::Byte_Code*>* _byte_code = new utils::LinkedList <byte_code::Byte_Code*>(), *_temp;
    byte_code::Byte_Code* _assign_expression =  getByteCodeOfExpressionId(__varAssign->expression_id);

    _temp = getByteCodeFromNode(
        __varAssign->value_before_assign, __crrnt, __comCntrl
    );

    _byte_code->join(
        _temp
    );

    _temp->destroy_content = 0; delete _temp;

    if (!__varAssign->operation_is_left) {

        _temp = getByteCodeFromNode(
            __varAssign->value, __crrnt, __comCntrl
        );

        _byte_code->join(
            _temp
        );

        _temp->destroy_content = 0; delete _temp;

    }

    _byte_code->add(_assign_expression);

    return _byte_code;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromPointerOperators(Ast_Node_Pointer_Operators* __pntrOprt, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node Pointer Operators <--");

    utils::LinkedList <byte_code::Byte_Code*>* _byte_code = new utils::LinkedList <byte_code::Byte_Code*>(), *_temp;
    byte_code::Byte_Code* _bc;
    int _off, _bcId, _lvl;

    _temp = getByteCodeFromNode(
        __pntrOprt->value, __crrnt, __comCntrl
    );

    _byte_code->join(_temp);

    _temp->destroy_content = 0; delete _temp;

    Type_Information _type = getTypeInformationOfNode(__pntrOprt->value);

    if ((_off = _type.pointer_level - __pntrOprt->pointer_level) < 0) new Compiler_Exception("Error pointer operators");

    _bcId = _off > _type.pointer_level ? BYTECODE_POINTER_OPERATIONS_UP : BYTECODE_POINTER_OPERATIONS_DOWN;
    _lvl = _bcId == BYTECODE_POINTER_OPERATIONS_UP ? _off - _type.pointer_level : _type.pointer_level - _off;

    if (!_lvl) return _byte_code;

    _bc  = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_bc) byte_code::Byte_Code(
        _bcId, _lvl
    );

    _byte_code->add(_bc);

    return _byte_code;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromParenthesis(Ast_Node_Parenthesis* __paren, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node Parenthesis <--"); 

    return getByteCodeFromExpression(__paren->value, __crrnt, __comCntrl);

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromFunctionCall(Ast_Node_Function_Call* __funcCall, Compiler_Code_Block* __crrnt, Compiler_Control* __comCntrl) {

    __comCntrl->printDebugInfo("--> Byte Code for node Function Call <--");

    utils::LinkedList <byte_code::Byte_Code*>* _byte_code = new utils::LinkedList <byte_code::Byte_Code*>(), *_temp;
    byte_code::Byte_Code* _func_call_byte_code;
    Ast_Node_Function_Declaration* _funcDecl;

    if (__funcCall->name_space) {

        __crrnt = __comCntrl->getCodeBlockFromNameSpace(__funcCall->name_space);

        if (!__crrnt) new Compiler_Exception("No variable is declared with given name - name space");

    }

    _funcDecl = __crrnt->getFunctionDeclaration(__funcCall->declaration_id);

    if (!_funcDecl) new Ast_Execption("Function not defined");

    __funcCall->function_declaration = _funcDecl;

    if(!parser_helper::confirmFunctionCallParameters(_funcDecl->parameters, __funcCall->parameters)) 
        new Compiler_Exception(
            "No same lenght of inputs in function declaration and function call\n" \
            "Or No same type size of inputs in function declaration and function call"
        );

    if (!_funcDecl->function_body) new Compiler_Exception("Forward not supported");

    for (int _ = 0; _ < __funcCall->parameters->count; _++) {

        _temp = parser::getByteCodeFromExpression(
            (*__funcCall->parameters)[_], __crrnt, __comCntrl
        );

        _byte_code->join(_temp);

        _temp->destroy_content = 0; delete _temp;

    }

    _func_call_byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new(_func_call_byte_code) byte_code::Byte_Code(
        BYTECODE_CALL, _funcDecl->body_pos
    );

    _byte_code->add(_func_call_byte_code);

    return _byte_code;

}
