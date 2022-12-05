#pragma once

#ifndef TOKEN_H
#define TOKEN_H

/* All supported tokens definitions */

#define TOKEN_ENDINSTRUCTION 1
#define TOKEN_OPENPARENTHESES 2
#define TOKEN_CLOSEPARENTHESES 3
#define TOKEN_OPENCURLYBRACKET 4
#define TOKEN_CLOSECURLYBRACKET 5
#define TOKEN_QUOTATIONMARK 6
#define TOKEN_QUESTIONMARK 7
#define TOKEN_TWOPOINTS 8
#define TOKEN_POINTER 9
#define TOKEN_ADDRESS 10
#define TOKEN_COMMA 11
#define TOKEN_COMMENT 12
#define TOKEN_HASHTAG 13

// Arithmetric Tokens // 

#define TOKEN_ADDITION 14
#define TOKEN_SUBTRACTION 15
#define TOKEN_MULTIPLICATION 16
#define TOKEN_DIVISION 17
#define TOKEN_MODULUS 18

// Relational Operators //

#define TOKEN_EQUALTO 19
#define TOKEN_NOTEQUALTO 20
#define TOKEN_GREATERTHAN 21
#define TOKEN_LESSTHAN 22
#define TOKEN_GREATERTHANEQUALTO 23
#define TOKEN_LESSTHANEQUALTO 24

// Logical Operators //

#define TOKEN_AND 25
#define TOKEN_OR 26
#define TOKEN_NOT 27

// Assignment Operators //

#define TOKEN_EQUAL 28
#define TOKEN_ADDITIONASSIGMENT 29
#define TOKEN_SUBTRACTIONASSIGMENT 30
#define TOKEN_MULTIPLICATIONASSIGMENT 31
#define TOKEN_DIVISIONASSIGMENT 32
#define TOKEN_MODULOSASSIGMENT 33
#define TOKEN_BITWISEANDASSIGMENT 34
#define TOKEN_BITWISEORASSIGMENT 35
#define TOKEN_BITWISEXORASSIGMENT 36
#define TOKEN_BITWISELEFTSHIFTASSIGMENT 37
#define TOKEN_BITWISERIGHTSHIFTASSIGMENT 38
#define TOKEN_INCREMENT 39
#define TOKEN_DECREMENT 40

// Bitwise Operators //

#define TOKEN_BITWISEAND 41
#define TOKEN_BITWISEOR 42
#define TOKEN_BITWISEXOR 43
#define TOKEN_BITWISENOT 44
#define TOKEN_BITWISELEFTSHIFT 45
#define TOKEN_BITWISERIGHTSHIFT 46

// Key Words Primatives Types //

#define TOKEN_TYPE_INT 47
#define TOKEN_TYPE_CHAR 48
#define TOKEN_TYPE_BOOL 49
#define TOKEN_TYPE_FLOAT 50
#define TOKEN_TYPE_DOUBLE 51
#define TOKEN_TYPE_VOID 52
#define TOKEN_TYPE_WCHAR 53

// Key Words //

#define TOKEN_ASM 54
#define TOKEN_NEW 55
#define TOKEN_SWITCH 56
#define TOKEN_AUTO 57
#define TOKEN_ELSE 58
#define TOKEN_OPERATOR 59
#define TOKEN_TEMPLATE 60
#define TOKEN_BREAK 61
#define TOKEN_ENUM 62
#define TOKEN_PRIVATE 63
#define TOKEN_THIS 64
#define TOKEN_CASE 65
#define TOKEN_EXTERN 66
#define TOKEN_PROTECTED 67
#define TOKEN_THROW 68
#define TOKEN_CATCH 69
#define TOKEN_PUBLIC 70
#define TOKEN_TRY 71
#define TOKEN_FOR 72
#define TOKEN_REGISTER 73
#define TOKEN_TYPEDEF 74
#define TOKEN_CLASS 75
#define TOKEN_FRIEND 76
#define TOKEN_RETURN 77
#define TOKEN_UNION 78
#define TOKEN_CONST 79
#define TOKEN_GOTO 80
#define TOKEN_CONTINUE 81
#define TOKEN_IF 82
#define TOKEN_VIRTUAL 83
#define TOKEN_DEFAULT 84
#define TOKEN_INLINE 85
#define TOKEN_SIZEOF 86
#define TOKEN_DELETE 87
#define TOKEN_STATIC 88
#define TOKEN_VOLATILE 89
#define TOKEN_DO 90
#define TOKEN_STRUCT 91
#define TOKEN_WHILE 92

// Others In identifiers //

#define TOKEN_IDENTIFIER 93
#define TOKEN_NUMBERINT 94
#define TOKEN_NUMBERLONG 95
#define TOKEN_NUMBERFLOAT 96
#define TOKEN_NUMBERDOUBLE 97
#define TOKEN_CHARACTER 98
#define TOKEN_STRING 99
#define TOKEN_ACCESSINGVARIABLE 100
#define TOKEN_ACCESSINGVARIABLEPOINTER 101

// Others again //

#define TOKEN_CONTRACT 102
#define TOKEN_NAMESPACE 103
#define TOKEN_TYPE_UNSIGNED 104
#define TOKEN_TYPE_UNSIGNED_INT 105
#define TOKEN_TYPE_UNSIGNED_CHAR 106

// Dont know //

#define TOKEN_NAMESPACE_OPERATOR 107
#define TOKEN_ELSE_IF 108
#define TOKEN_END_CODE 109
#define TOKEN_SINGLE_QUOTE 110
#define TOKEN_STRING_SINGLE_QUOTE 111
#define TOKEN_NAMESPACE_OPERATOR_GERAL 112
#define TOKEN_INCREMENT_LEFT 113
#define TOKEN_DECREMENT_LEFT 114

namespace parser {

    struct Token {
        char* phr; // Hold the phrase if is not a "special" token
        int id; // Token id
        ~Token();
        Token(int, char*);
    };

    int getTokenSymbOneChar(int);
    int getTokenSymbTwoChars(int, int);
    int getTokenSymbThreeChars(int, int, int);
    int getTokenIdKeyWord(char**);
    int getTokenIdIdetifier(char*);

    bool isIdentifierType(parser::Token*);
    bool isImplicitValue(parser::Token*);
    bool isPrimativeType(parser::Token*);
    bool isAssignment(parser::Token*);
    bool isSingleAssignment(parser::Token*);
    bool isExpressionOperator(parser::Token*);
    bool isAccessingExpression(parser::Token*);

    bool isSingleAssignment(int);

}


#endif