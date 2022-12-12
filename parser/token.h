#pragma once

#ifndef TOKEN_H
#define TOKEN_H

#include "./token_definitions.h"

namespace parser {

    struct Token { char* phr; int id, line_position, collumn_position; ~Token(); Token(int, char*); };

    int getTokenSymbOneChar(int);
    int getTokenSymbTwoChars(int, int);
    int getTokenSymbThreeChars(int, int, int);

    int getTokenIdKeyWord(char**);

    int getTokenIdImplicitValue(char*);

    bool isPrimitiveTokenId(int);
    bool isImplicitValue(int);
    bool isExpressionOperator(int);

    

}

#endif