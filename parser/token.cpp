#include "./token.h"

#include "./../utils/commonFunctions.h"

#include <iostream>
#include <string.h>

parser::Token::~Token() { free(phr); }

parser::Token::Token(int _id, char* _phr) : id(_id), phr(_phr) {}

int parser::getTokenSymbOneChar(int _f) {

    switch (_f)
    {
        case ';': return TOKEN_ENDINSTRUCTION;
        case '(': return TOKEN_OPENPARENTHESES;
        case ')': return TOKEN_CLOSEPARENTHESES;
        case '{': return TOKEN_OPENCURLYBRACKET;
        case '}': return TOKEN_CLOSECURLYBRACKET;
        case '"': return TOKEN_QUOTATIONMARK;
        case '?': return TOKEN_QUESTIONMARK;
        case ':': return TOKEN_TWOPOINTS;
        case '.': return TOKEN_ACCESSINGVARIABLE;
        case '*': return TOKEN_POINTER;
        case '&': return TOKEN_ADDRESS;
        case ',': return TOKEN_COMMA;
        case '#': return TOKEN_HASHTAG;
        case '\'': return TOKEN_SINGLE_QUOTE;

        case '+': return TOKEN_ADDITION;
        case '-': return TOKEN_SUBTRACTION;
        case '/': return TOKEN_DIVISION;
        case '%': return TOKEN_MODULUS;

        case '=': return TOKEN_EQUAL;

        case '>': return TOKEN_GREATERTHAN;
        case '<': return TOKEN_LESSTHAN;

        case '!': return TOKEN_NOT;

        case '|': return TOKEN_BITWISEOR;
        case '^': return TOKEN_BITWISEXOR;
        case '~': return TOKEN_BITWISENOT;

    }

    return 0;

}

int parser::getTokenSymbTwoChars(int _f, int _s) {

    switch (_f) {

        case ':':
            switch (_s)
            {
            case ':': return TOKEN_NAMESPACE_OPERATOR;
            
            default: return 0;
            }

        case '=':
            switch (_s)
            {
                case '=': return TOKEN_EQUALTO;
                default: return 0;
            }
    
        case '/':
            switch (_s) {              
                case '/': return TOKEN_COMMENT;
                case '=': return TOKEN_DIVISIONASSIGMENT;
                default: return 0;
            }

        case '!':
            switch (_s) {
                case '=': return TOKEN_NOTEQUALTO;
                default: return 0;
            }

        case '>':
            switch (_s) {         
                case '=': return TOKEN_GREATERTHANEQUALTO;
                case '>': return TOKEN_BITWISERIGHTSHIFT;
                default: return 0;
            }
        case '<':
            switch (_s) {               
                case '=': return TOKEN_LESSTHANEQUALTO;
                case '<': return TOKEN_BITWISELEFTSHIFT;
                default: return 0;
            }
        case '&':
            switch (_s) {           
                case '&': return TOKEN_AND;
                case '=': return TOKEN_BITWISEANDASSIGMENT;
                default: return 0;
            }
        case '|':
            switch (_s) {           
                case '|': return TOKEN_OR;
                case '=': return TOKEN_BITWISEORASSIGMENT;
                default: return 0;
            }
        case '+':
            switch (_s) {           
                case '+': return TOKEN_INCREMENT;
                case '=': return TOKEN_ADDITIONASSIGMENT;
                default: return 0;
            }
        case '-':
            switch (_s) {
                case '>': return TOKEN_ACCESSINGVARIABLEPOINTER;
                case '-': return TOKEN_DECREMENT;
                case '=': return TOKEN_SUBTRACTIONASSIGMENT;
                default: return 0;
            }
        case '*':
            switch (_s) {
                case '=': return TOKEN_MULTIPLICATIONASSIGMENT;
                default: return 0;
            }
        case '%':
            switch (_s) {           
                case '=': return TOKEN_MODULOSASSIGMENT;
                default: return 0;
            }
        case '^':
            switch (_s) {           
                case '=': return TOKEN_BITWISEXORASSIGMENT;
                default: return 0;
            }
    }

    return 0;

}

int parser::getTokenSymbThreeChars(int _f, int _s, int _t) {

    switch (_f) {
        case '<':   
            switch (_s) {
                case '<':                   
                    switch (_t) {
                        case '=': return TOKEN_BITWISELEFTSHIFTASSIGMENT;
                        default: return 0;
                    }
                default: return 0;
            }
        case '>':           
            switch (_s) {
                case '>':                   
                    switch (_t) {
                        case '=': return TOKEN_BITWISERIGHTSHIFTASSIGMENT;  
                        default: return 0;
                    }
            default: return 0;
            }
    }

    return 0;

}

int parser::getTokenIdKeyWord(char** _) {

    /* Types */
    if (!strncmp( (const char*) *_, "int", 3)) { *_+= 3; return TOKEN_TYPE_INT; }
    if (!strncmp( (const char*) *_, "char", 4)) { *_+= 4; return TOKEN_TYPE_CHAR; }
    if (!strncmp( (const char*) *_, "bool", 4)) { *_+= 4; return TOKEN_TYPE_BOOL; }
    if (!strncmp( (const char*) *_, "float", 5)) { *_+= 5; return TOKEN_TYPE_FLOAT; }
    if (!strncmp( (const char*) *_, "double", 6)) { *_+= 6; return TOKEN_TYPE_DOUBLE; }
    if (!strncmp( (const char*) *_, "void", 4)) { *_+= 4; return TOKEN_TYPE_VOID; }
    if (!strncmp( (const char*) *_, "wchar", 5)) { *_+= 5; return TOKEN_TYPE_WCHAR; }
    if (!strncmp( (const char*) *_, "unsigned", 8)) { *_+= 5; return TOKEN_TYPE_UNSIGNED; }

    /* Others */
    if (!strncmp( (const char*) *_, "struct", 6)) { *_+= 6; return TOKEN_STRUCT; }
    if (!strncmp( (const char*) *_, "contract", 8)) { *_+= 8; return TOKEN_CONTRACT; }
    if (!strncmp( (const char*) *_, "namespace", 9)) { *_+= 9; return TOKEN_NAMESPACE; }


    /* Control Structures */
    if (!strncmp( (const char*) *_, "if", 2)) { *_+=2; return TOKEN_IF; }
    if (!strncmp( (const char*) *_, "else if", 7)) { *_+=7; return TOKEN_ELSE_IF; }
    if (!strncmp( (const char*) *_, "else", 4)) { *_+=4; return TOKEN_ELSE; }


    return 0;

}

int parser::getTokenIdIdetifier(char* _) {

    if (utils::isInt(_)) return TOKEN_NUMBERINT;
    if (utils::isLong(_)) return TOKEN_NUMBERLONG;
    if (utils::isDouble(_)) return TOKEN_NUMBERDOUBLE;

    return TOKEN_IDENTIFIER;

}


bool parser::isIdentifierType(parser::Token* _) {

    return (_->id >= TOKEN_IDENTIFIER  && _->id <= TOKEN_ACCESSINGVARIABLEPOINTER) || _->id == TOKEN_CLOSEPARENTHESES;

}

bool parser::isImplicitValue(parser::Token* _) { return _->id >= TOKEN_NUMBERINT && _->id <= TOKEN_STRING; }

bool parser::isPrimativeType(parser::Token* _) { return (_->id >= TOKEN_TYPE_INT && _->id <= TOKEN_TYPE_WCHAR) || 
    (_->id >= TOKEN_TYPE_UNSIGNED_INT && _->id <= TOKEN_TYPE_UNSIGNED_CHAR); }

bool parser::isAssignment(parser::Token* _) { return _->id >= TOKEN_EQUAL && _->id <= TOKEN_DECREMENT; }

bool parser::isSingleAssignment(parser::Token*_) { return _->id == TOKEN_INCREMENT || _->id == TOKEN_DECREMENT; }

bool parser::isExpressionOperator(parser::Token* _) { // Miss things

    return _->id >= TOKEN_ADDITION && _->id <= TOKEN_MODULUS;

}

bool parser::isAccessingExpression(parser::Token* _) { return _->id == TOKEN_ACCESSINGVARIABLE || _->id == TOKEN_ACCESSINGVARIABLEPOINTER; }


bool parser::isSingleAssignment(int __id) {

    switch (__id)
    {
    case TOKEN_INCREMENT: case TOKEN_DECREMENT: case TOKEN_INCREMENT_LEFT: case TOKEN_DECREMENT_LEFT: return 1;
    default: break;
    }

    return 0;

}


