#pragma once

#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

#define AST_NODE_NAME_SPACE 1
#define AST_NODE_CODE_BLOCK 2
#define AST_NODE_VARIABLE_DECLARATION 3
#define AST_NODE_FUNCTION_DECLARATION 4
#define AST_NODE_STRUCT_DECLARATION 5
#define AST_NODE_EXPRESSION 6
#define AST_NODE_VALUE 7
#define AST_NODE_VARIABLE 8
#define AST_NODE_ASSIGNMENT 9
#define AST_NODE_FUNCTION_CALL 10
#define AST_NODE_POINTER_OPERATOR 11
#define AST_NODE_PARENTHESIS 12

#define GET_NODE_TYPE_TOKEN_CLOSE_CURLY_BRACKET 15
#define GET_NODE_TYPE_TOKEN_END_INSTRUCTION 16
#define GET_NODE_TYPE_TOKEN_CLOSE_PARENTHESIS 17

#endif