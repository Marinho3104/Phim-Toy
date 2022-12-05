#include "./compiler_byte_code_converted.h"

#include "./../byteCode/byte_code_definitions.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./compiler.h"
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

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNode(Compiler_Control* __compiler_control, Compiler_Code_Block* __current, Ast_Node* __node) {

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

        case AST_NODE_FUNCTION_DECLARATION:

            parser::getByteCodeFromFunctionDeclaration(
                (Ast_Node_Function_Declaration*) __node,
                __current, __compiler_control
            );

            goto rtr;

        case AST_NODE_STRUCT_DECLARATION:

            parser::getByteCodeFromStructDeclaration(
                (Ast_Node_Struct_Declaration*) __node,
                __current, __compiler_control
            );

            goto rtr;

        case AST_NODE_VALUE:

            _byte_code->add(
                parser::getByteCodeFromValue(
                    (Ast_Node_Value*) __node,
                    __current, __compiler_control
                )
            );

            goto rtr;

        case AST_NODE_VARIABLE:

            _byte_code->add(
                parser::getByteCodeFromVariable(
                    (Ast_Node_Variable*) __node,
                    __current, __compiler_control
                )
            );

            goto rtr;
        
        default: break;
        
    }

    delete _byte_code;

    switch (__node->node_id)
    {
    case AST_NODE_EXPRESSION:
        
        _byte_code = parser::getByteCodeFromExpression(
            (Ast_Node_Expression*) __node,
            __current, __compiler_control
        );

        goto rtr;

    case AST_NODE_ASSIGNMENT:

        _byte_code = parser::getByteCodeFromAssignment(
            (Ast_Node_Assignment*) __node,
            __current, __compiler_control
        );

        goto rtr;

    case AST_NODE_PARENTHESIS:

        _byte_code = parser::getByteCodeFromParenthesis(
            (Ast_Node_Parenthesis*) __node,
            __current, __compiler_control
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

void parser::getByteCodeFromFunctionDeclaration(Ast_Node_Function_Declaration* __function_declaration, Compiler_Code_Block* __current, Compiler_Control* __compiler_control) {

    __compiler_control->printDebugInfo("--> Byte Code From Function Declaration <--");

    if (__current->compiler_declarations->isDeclared(__function_declaration->declaration_id)) new Compiler_Exception("Redeclaration of variable");

    __current->compiler_declarations->function_declarations->add(__function_declaration);

    __function_declaration->body_pos = Compiler_Code_Block::generate(__compiler_control, __function_declaration, NULL);

    __compiler_control->printDebugInfo("--> Byte Code From Function Declaration End <--");

}

void parser::getByteCodeFromStructDeclaration(Ast_Node_Struct_Declaration* __struct_declaration, Compiler_Code_Block* __current, Compiler_Control* __compiler_control) {

    __compiler_control->printDebugInfo("--> Byte Code From Struct Declaration <--");

    if (__current->compiler_declarations->isDeclared(__struct_declaration->declaration_id)) new Compiler_Exception("Redeclaration of variable");

    __current->compiler_declarations->struct_declarations->add(__struct_declaration);

    Compiler_Code_Block::generate(__compiler_control, __struct_declaration, __current);

    __compiler_control->printDebugInfo("--> Byte Code From Struct Declaration End <--");

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
        _ = parser::getByteCodeFromNode(__comCntrl, __crrnt, __exp->first_expression);
        _rtr->join(
            _
        );
        _->destroy_content = 0;
        delete _;
    }
    else _c = true;

    _frstOp = __exp->expression_operator_id;
    _frstOpPri = getExpressionPriority(_frstOp);

    if (__exp->second_expression) {

        __exp = (parser::Ast_Node_Expression*) __exp->second_expression;

        if (
            getExpressionPriority(__exp->expression_operator_id) <= _frstOpPri
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

            _ = parser::getByteCodeFromNode(__comCntrl, __crrnt, __exp->first_expression);
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

byte_code::Byte_Code* parser::getByteCodeFromValue(Ast_Node_Value* __value, Compiler_Code_Block* __current, Compiler_Control* __compiler_control) {

    __compiler_control->printDebugInfo("--> Byte Code From Value <--");

    byte_code::Byte_Code* _byteCode = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byteCode) byte_code::Byte_Code(
        BYTECODE_MEM_STACK_LOAD_IMPLICIT, __value->value_position
    );

    __compiler_control->printDebugInfo("--> Byte Code From Value End <--");
    
    return _byteCode;

}

byte_code::Byte_Code* parser::getByteCodeFromVariable(Ast_Node_Variable* __variable, Compiler_Code_Block* __current, Compiler_Control* __compiler_control) {

    __compiler_control->printDebugInfo("--> Byte Code From Variable <--");

    Ast_Node_Variable_Declaration* _variable_declaration;

    if (__variable->name_space) {

        Compiler_Code_Block* _compiler_code_block = __compiler_control->getCompilerCodeBlockFromNameSpace(__variable->name_space);

        if (!_compiler_code_block) new Compiler_Exception("Error getting name space");

        _variable_declaration = _compiler_code_block->compiler_declarations->getVariableDeclaration(__variable->declaration_id);

    } else _variable_declaration = __current->getVariableDeclaration(__variable->declaration_id);

    if (!_variable_declaration) new Compiler_Exception("No variable declared with given name");

    byte_code::Byte_Code* _loadVar = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
    
    new (_loadVar) byte_code::Byte_Code(
        __variable->is_global ? BYTECODE_LOAD_VARIABLE_GLOBAL : BYTECODE_LOAD_VARIABLE,
        __variable->declaration_id
    );

    __compiler_control->printDebugInfo("--> Byte Code From Variable End <--");

    return _loadVar;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromAssignment(Ast_Node_Assignment* __assignment, Compiler_Code_Block* __current, Compiler_Control* __compiler_control) {

    __compiler_control->printDebugInfo("--> Byte Code From Variable Assignment <--");

    utils::LinkedList <byte_code::Byte_Code*>* _byte_code = new utils::LinkedList <byte_code::Byte_Code*>(), *_temp;

    _temp = getByteCodeFromNode(__compiler_control, __current, __assignment->value_before_assign);

    _byte_code->join(_temp); 

    _temp->destroy_content = 0; delete _temp;

    if (!parser::isSingleAssignment(__assignment->expression_operator_id)) {

        _temp = getByteCodeFromNode(__compiler_control, __current, __assignment->value_after_assign);

        _byte_code->join(_temp); 

        _temp->destroy_content = 0; delete _temp;

    }

    _byte_code->add(
        getByteCodeOfExpressionId(__assignment->expression_operator_id)
    );

    __compiler_control->printDebugInfo("--> Byte Code From Variable Assignment End <--");
    
    return _byte_code;

}

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromParenthesis(Ast_Node_Parenthesis* __parenthesis, Compiler_Code_Block* __current, Compiler_Control* __compiler_control) {

    __compiler_control->printDebugInfo("--> Byte Code From Parenthesis <--");

    utils::LinkedList <byte_code::Byte_Code*>* _byte_code = getByteCodeFromNode(__compiler_control, __current, __parenthesis->value);

    __compiler_control->printDebugInfo("--> Byte Code From Parenthesis End <--");

    return _byte_code;

}




