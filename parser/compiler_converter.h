#pragma once

#ifndef PARSER_COMPILER_CONVERTER_H
#define PARSER_COMPILER_CONVERTER_H

#include "./ast_nodes.h" // Ast Nodes

namespace parser { struct Compiler_Control; struct Storage; }
namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser_helper {

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

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNode(parser::Ast_Node*, parser::Compiler_Control*);
    
    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeValue(parser::Ast_Node_Value*, parser::Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeVariable(parser::Ast_Node_Variable*, parser::Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeExpression(parser::Ast_Node_Expression*, parser::Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeExpressionS(parser::Ast_Node_Expression*&, bool&, parser::Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeVariableDeclaration(parser::Ast_Node_Variable_Declaration*, parser::Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeVariableAssign(parser::Ast_Node_Variable_Assignment*, parser::Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeParenthesis(parser::Ast_Node_Parenthesis*, parser::Compiler_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodePointerOperations(parser::Ast_Node_Pointer_Operators*, parser::Compiler_Control*);
    
    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeFunctionDeclaration(parser::Ast_Node_Function_Declaration*, parser::Compiler_Control*);

}

#endif