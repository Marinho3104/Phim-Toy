#include "./parser_helper.h"

#include "./definitions.h" // Parser definitions
#include "./token.h" // Token id

#include <iostream>

int parser_helper::getSizePrimitiveType(int _tkId) {

    switch (_tkId)
    {
    case TOKEN_TYPE_INT: return PRIMATIVES_TYPE_INT_SIZE;
    case TOKEN_TYPE_VOID: return PRIMATIVES_TYPE_VOID_SIZE;
    
    default:
        break;
    }

    return -1;

}

int parser_helper::getSizeImplicitValue(int _tkId) {

    switch (_tkId)
    {
    case TOKEN_NUMBERINT: return PRIMATIVES_TYPE_INT_SIZE;
    case TOKEN_STRING: return 0; // TODO Change
    
    default:
        break;
    }

    return -1;

}

int parser_helper::getTokenIdTypeFromTokenIdImplicitValue(int _tkId) {

    switch (_tkId)
    {
    case TOKEN_NUMBERINT: return TOKEN_TYPE_INT;
    
    default:
        break;
    }

    return -1;

}




