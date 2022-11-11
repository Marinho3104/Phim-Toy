#pragma once

#ifndef BYTE_CODE_DEFINITIONS_H
#define BYTE_CODE_DEFINITIONS_H

#define BYTECODE_MEM_STACK_LOAD_CONST 0 // Load the argument value into stack memory and to stack (arg -> actual value pos in table constsValue) NodeValue

#define BYTECODE_ADDITION 1
#define BYTECODE_SUBTRACTION 2
#define BYTECODE_MULTIPLICATION 3
#define BYTECODE_DIVISION 4
#define BYTECODE_MODULOS 5


#define BYTECODE_LOAD_NAME 6

#define BYTECODE_MEM_STACK_LOAD 7
#define BYTECODE_ASSIGN 8

#define BYTECODE_LOAD_VARIABLE 9



#endif