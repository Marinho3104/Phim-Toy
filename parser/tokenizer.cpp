#include "./tokenizer.h"

#include "./../utils/commonFunctions.h"
#include "./../utils/linkedList.h"
#include "./token.h"

#include <iostream>

parser::Tokenizer_Control_Exception::Tokenizer_Control_Exception(char* _description) : description(_description) {

    std::cout << "Exception: " << _description << std::endl;

    exit(-1);

}

parser::Tokenizer_Control::~Tokenizer_Control() {

    delete tokens;
    
}

parser::Tokenizer_Control::Tokenizer_Control(char* _srcCode) : srcCode(_srcCode) {

    tokens = new utils::LinkedList <parser::Token*>();

}

void parser::Tokenizer_Control::addNewToken(parser::Token* _) {

    if (tokens->last != NULL && _->id != TOKEN_IDENTIFIER && 
        (tokens->last->object->id == TOKEN_ACCESSINGVARIABLE || tokens->last->object->id == TOKEN_ACCESSINGVARIABLEPOINTER)
    ) new Tokenizer_Control_Exception("Expected identifier after trying to access field");


    tokens->add(_);
}

bool parser::Tokenizer_Control::setTokenSymbol() {

    char* _inWrd;
    parser::Token* _tk = NULL;
    int _tkId, _tkId1, _tkId2;

    // Checks first char
    if (_tkId = parser::getTokenSymbOneChar(*srcCode)) {

        // Token is '"' collects all string until reach other '"'
        if (_tkId == TOKEN_QUOTATIONMARK) {

            bool _backSlash = 0;
            srcCode++;
            _inWrd = srcCode;

            while(
                *srcCode != '"' || _backSlash
            ) { _backSlash = (*srcCode == '\\') ? 1 : 0; srcCode++; } /* Miss handle with \ in that case TODO :() */
            
            // Creates token
            int _s = srcCode - _inWrd;
            char* _phr = (char*) malloc(sizeof(char) * _s + 1);

            for (int __ = 0; __ < _s;__++) 
                _phr[__] = _inWrd[__];

            _phr[_s] = 0;

            srcCode++;

            addNewToken(
                new parser::Token(
                    TOKEN_STRING, _phr
                )
            );

        }

    }

    // Checks second char
    if (_tkId1 = parser::getTokenSymbTwoChars(*srcCode, *(srcCode + 1))) {

        // Token is '//' just ignore all line 
        if (_tkId1 == TOKEN_COMMENT) {
            do srcCode++; while(*srcCode != '\n'); srcCode++;
            return 1;
        } 

    }

    // Checks third char
    if (_tkId2 = parser::getTokenSymbThreeChars(*srcCode, *(srcCode + 1), *(srcCode + 2))) 
        
        { srcCode+=3; _tk = (parser::Token*) malloc(sizeof(parser::Token)); new(_tk) parser::Token(_tkId2, NULL); }  

    else if (_tkId1) { srcCode+=2; _tk = (parser::Token*) malloc(sizeof(parser::Token)); new(_tk) parser::Token(_tkId1, NULL); }

    else if (_tkId) { srcCode++; _tk = (parser::Token*) malloc(sizeof(parser::Token)); new(_tk) parser::Token(_tkId, NULL); }

    if (_tk) { addNewToken(_tk); return 1; }

    return 0;

}

bool parser::Tokenizer_Control::setTokenKeyWord() {

    parser::Token* _tk = NULL;
    int _tkId;

    if (_tkId = parser::getTokenIdKeyWord(&srcCode)) { _tk = (parser::Token*) malloc(sizeof(parser::Token)); new(_tk) parser::Token(_tkId, NULL); }

    if (_tk) addNewToken(_tk);

    return _tkId;

}

void parser::Tokenizer_Control::setTokenIdentifier() {

    parser::Token* _tk, *_tkAdd = NULL;

    _tk = (parser::Token*) malloc(sizeof(parser::Token));
    char* _wrd = srcCode;
    int _probAccess;

    do { 
        srcCode++; 
        if (!*srcCode) new Tokenizer_Control_Exception("Unexpected token at the end.");

        if (_probAccess = parser::getTokenSymbOneChar(*srcCode)) {

            if (_probAccess == TOKEN_ACCESSINGVARIABLE) 
                { _tkAdd = (parser::Token*) malloc(sizeof(parser::Token)); new(_tkAdd) parser::Token(_probAccess, NULL);}

            else if (
                _probAccess == TOKEN_SUBTRACTION &&
                (_probAccess = parser::getTokenSymbTwoChars(*srcCode, *(srcCode + 1)))
                == TOKEN_ACCESSINGVARIABLEPOINTER
            ) { _tkAdd = (parser::Token*) malloc(sizeof(parser::Token)); new(_tkAdd) parser::Token(_probAccess, NULL); }

            break;

        }

    } while(
        *srcCode != ' '
    );

    int _s = srcCode - _wrd;

    char* _str = (char*) malloc(sizeof(char) * _s + 1);

    for (int _ = 0; _ < _s; _++) 

        _str[_] = *(_wrd++);

    _str[_s] = 0;
    
    new(_tk) Token(
        parser::getTokenIdIndetifier(_str),
        _str
    );
    
    tokens->add(_tk);

    if (_tkAdd) { addNewToken(_tkAdd); srcCode += (_tkAdd->id == TOKEN_ACCESSINGVARIABLE) ? 1 : 2; }

}

void parser::Tokenizer_Control::setNewToken() {

    if (
        setTokenSymbol()
    ) {

        if (tokens->last->object->id == TOKEN_ADDRESS && parser::isIdentifierType(tokens->last->previous->object)) tokens->last->object->id = TOKEN_BITWISEAND;
        else if (tokens->last->object->id == TOKEN_POINTER && parser::isIdentifierType(tokens->last->previous->object)) tokens->last->object->id = TOKEN_MULTIPLICATION;

    } else if (setTokenKeyWord());

    else setTokenIdentifier();

}

void parser::Tokenizer_Control::generateTokens() {

    while(*srcCode != 0) {
        if (*srcCode == ' ') { 
            if (tokens->last->object->id == TOKEN_ACCESSINGVARIABLE || tokens->last->object->id == TOKEN_ACCESSINGVARIABLEPOINTER)
                new Tokenizer_Control_Exception("Expected identifier after trying to access field");
            srcCode++; continue; }
        setNewToken();
    }

    if (parser::isIdentifierType(tokens->last->object)) new Tokenizer_Control_Exception("Expected ; token at the end"); 

    parser::Token* _fnl = (parser::Token*) malloc(sizeof(parser::Token));
    new (_fnl) parser::Token(TOKEN_END_CODE, NULL);

    tokens->add(_fnl);

}
