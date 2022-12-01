#pragma once

#ifndef PARSER_COMPILER_BYTE_CODE_CONVERT   
#define PARSER_COMPILER_BYTE_CODE_CONVERT

#include "./ast_nodes.h"

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Compiler_Code_Block;
    struct Compiler_Control;

    /* Return value by expression value
    *   --> return table <--
    *       1 -> ()
    *       2 -> * / % ++ --
    *       3 -> + -
    *       4 -> & | ^ ~ << >>
    *       5 -> && || !
    */
    int getExpressionPriority(int);

    byte_code::Byte_Code* getByteCodeOfExpressionId(int);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNode(Ast_Node*, Compiler_Code_Block*, Compiler_Control*);

    byte_code::Byte_Code* getByteCodeFromVariableDeclaration(Ast_Node_Variable_Declaration*, Compiler_Code_Block*, Compiler_Control*);

    void getByteCodeFromFunctionDeclaration(Ast_Node_Function_Declaration*, Compiler_Code_Block*, Compiler_Control*);

    void getByteCodeFromStructDeclaration(Ast_Node_Struct_Declaration*, Compiler_Code_Block*, Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromExpression(Ast_Node_Expression*, Compiler_Code_Block*, Compiler_Control*);
    
    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromExpressionS(Ast_Node_Expression*&, Compiler_Code_Block*, Compiler_Control*, bool&);

    byte_code::Byte_Code* getByteCodeFromValue(Ast_Node_Value*, Compiler_Code_Block*, Compiler_Control*);

    byte_code::Byte_Code* getByteCodeFromVariable(Ast_Node_Variable*, Compiler_Code_Block*, Compiler_Control*);

}

#endif