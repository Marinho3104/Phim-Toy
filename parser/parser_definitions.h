#pragma once

#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

// Ast Id's //

#define AST_NODE_NAME_SPACE 0
#define AST_NODE_CODE_BLOCK 1
#define AST_NODE_VARIABLE_DECLARATION 2
#define AST_NODE_FUNCTION_DECLARATION 3
#define AST_NODE_STRUCT_DECLARATION 4
#define AST_NODE_EXPRESSION 5
#define AST_NODE_VALUE 6
#define AST_NODE_VARIABLE 7
#define AST_NODE_ASSIGNMENT 8
#define AST_NODE_POINTER_OPERATOR 9
#define AST_NODE_PARENTHESIS 10
#define AST_NODE_FUNCTION_CALL 11
#define AST_NODE_ACCESSING 12

// #define AST_NODE_NAME_SPACE_CHANGE 10
// #define AST_NODE_END 12

// Ast Id's //

// Ast Node Code Block Environemt Type //

// #define AST_NODE_CODE_BLOCK_ENVIRONMENT_NAME_SPACE 0
#define AST_NODE_CODE_BLOCK_ENVIRONMENT_SINGLE 0
#define AST_NODE_CODE_BLOCK_ENVIRONMENT_FUNCTION 1
#define AST_NODE_CODE_BLOCK_ENVIRONMENT_STRUCT 2

// #define AST_NODE_CODE_BLOCK_ENVIRONMENT_NORMAL 0
// #define AST_NODE_CODE_BLOCK_ENVIRONMENT_GLOBAL 1

// Ast Node Code Block Environemt Type //

// Primatives Types Sizes //

#define PRIMITIVES_TYPE_POINTER_SIZE 2
#define PRIMATIVES_TYPE_INT_SIZE 4
#define PRIMATIVES_TYPE_VOID_SIZE 0

// Primatives Types Sizes //

#endif