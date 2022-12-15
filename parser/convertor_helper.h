#pragma once

#ifndef PARSER_CONVERTOR_HELPER_H
#define PARSER_CONVERTOR_HELPER_H

#include "./ast_nodes.h"

namespace utils { template <typename> struct Linked_List; }
namespace byte_code { struct Byte_Code; }
namespace parser { struct Convertor; }

namespace parser_helper {

    int getExpressionPriority(int);

    byte_code::Byte_Code* getByteCodeOfExpressionId(int);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeFromNode(parser::Ast_Node*);

    byte_code::Byte_Code* getByteCodeFromNodeVariableDeclaration(parser::Ast_Node_Variable_Declaration*);

    void getByteCodeFromNodeFunctionDeclaration(parser::Ast_Node_Function_Declaration*);

    void getByteCodeFromNodeStructDeclaration(parser::Ast_Node_Struct_Declaration*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeFromExpression(parser::Ast_Node_Expression*);
    
    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeFromExpressionS(parser::Ast_Node_Expression*&, bool&);
    
    byte_code::Byte_Code* getByteCodeFromValue(parser::Ast_Node_Value*);

    byte_code::Byte_Code* getByteCodeFromVariable(parser::Ast_Node_Variable*);

    byte_code::Byte_Code* getByteCodeFromFunctionCall(parser::Ast_Node_Function_Call*);




}

#endif