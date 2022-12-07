#pragma once

#ifndef PARSER_COMPILER_BYTE_CODE_CONVERTED_H
#define PARSER_COMPILER_BYTE_CODE_CONVERTED_H

#include "./ast_nodes.h"

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Compiler_Code_Block;
    struct Compiler_Control;
    struct Ast_Node;

    byte_code::Byte_Code* getByteCodeOfExpressionId(int);

    int getExpressionPriority(int);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNode(Compiler_Control*, Compiler_Code_Block*, Ast_Node*);

    byte_code::Byte_Code* getByteCodeFromVariableDeclaration(Ast_Node_Variable_Declaration*, Compiler_Code_Block*, Compiler_Control*);

    void getByteCodeFromFunctionDeclaration(Ast_Node_Function_Declaration*, Compiler_Code_Block*, Compiler_Control*);

    void getByteCodeFromStructDeclaration(Ast_Node_Struct_Declaration*, Compiler_Code_Block*, Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromExpression(Ast_Node_Expression*, Compiler_Code_Block*, Compiler_Control*);
    
    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromExpressionS(Ast_Node_Expression*&, Compiler_Code_Block*, Compiler_Control*, bool&);

    byte_code::Byte_Code* getByteCodeFromValue(Ast_Node_Value*, Compiler_Code_Block*, Compiler_Control*);

    byte_code::Byte_Code* getByteCodeFromVariable(Ast_Node_Variable*, Compiler_Code_Block*, Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromAssignment(Ast_Node_Assignment*, Compiler_Code_Block*, Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromFunctionCall(Ast_Node_Function_Call*, Compiler_Code_Block*, Compiler_Control*);
    
    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromPointerOperator(Ast_Node_Pointer_Operator*, Compiler_Code_Block*, Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromParenthesis(Ast_Node_Parenthesis*, Compiler_Code_Block*, Compiler_Control*);

}

#endif
