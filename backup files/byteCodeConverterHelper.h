#pragma once

#ifndef BYTE_CODE_CONVERTER_HELPER_H
#define BYTE_CODE_CONVERTER_HELPER_H

#include "./astNodes.h"

namespace utils { template <typename> struct LinkedList; }
namespace parser { struct Byte_Code_Converter_Control; }
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
    int getExpressionPriority(int, utils::LinkedList<int>*);

    byte_code::Byte_Code* getByteCodeOfExpressionId(int);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeValue(parser::Ast_Node_Value*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeVariable(parser::Ast_Node_Variable*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNode(parser::Ast_Node*, parser::Byte_Code_Converter_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeExpressison(parser::Ast_Node_Expression*, parser::Byte_Code_Converter_Control*);
    
    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeExpressisonS(parser::Ast_Node_Expression**, parser::Byte_Code_Converter_Control*, bool&);
    
    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeParenthesis(parser::Ast_Node_Parenthesis*, parser::Byte_Code_Converter_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeVariableDeclaration(
        parser::Ast_Node_Variable_Declaration*, parser::Byte_Code_Converter_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodePointerOperators(parser::Ast_Node_Pointer_Operators*, parser::Byte_Code_Converter_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeVariableAssign(parser::Ast_Node_Variable_Assignment*, parser::Byte_Code_Converter_Control*);

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNodeFunctionCall(parser::Ast_Node_Function_Call*, parser::Byte_Code_Converter_Control*);

}

#endif