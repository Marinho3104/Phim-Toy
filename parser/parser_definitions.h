#pragma once

#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

// Ast Id's //

#define AST_NODE_CODE_BLOCK 0
#define AST_NODE_EXPRESSION 1
#define AST_NODE_VALUE 2
#define AST_NODE_VARIABLE_DECLARATION 3
#define AST_NODE_VARIABLE 4
#define AST_NODE_POINTER_OPERATORS 5
#define AST_NODE_VARIABLE_ASSIGNMENT 6
#define AST_NODE_PARENTHESIS 7
#define AST_NODE_FUNCTION_DECLARATION 8
#define AST_NODE_FUNCTION_CALL 9
#define AST_NODE_STRUCT_DECLARATION 10
#define AST_NODE_END 11

// Ast Id's //

// Ast Node Code Block Environemt Type //

#define AST_NODE_CODE_BLOCK_ENVIRONMENT_NORMAL 0
#define AST_NODE_CODE_BLOCK_ENVIRONMENT_GLOBAL 1
#define AST_NODE_CODE_BLOCK_ENVIRONMENT_FUNCTION 2
#define AST_NODE_CODE_BLOCK_ENVIRONMENT_STRUCT 3

// Ast Node Code Block Environemt Type //

// Primatives Types Sizes //

#define PRIMITIVES_TYPE_POINTER_SIZE 2
#define PRIMATIVES_TYPE_INT_SIZE 4
#define PRIMATIVES_TYPE_VOID_SIZE 0

// Primatives Types Sizes //

#endif