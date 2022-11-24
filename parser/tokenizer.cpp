#include "./tokenizer.h"

#include "./../utils/commonFunctions.h"
#include "./../utils/linkedList.h"
#include "./token.h"

#include <iostream>

parser::Tokenizer_Control_Exception::Tokenizer_Control_Exception(char* __descr) : description(__descr) {

    std::cout << "Tokenizer error: " << description << std::endl;

    exit(-1);

}

parser::Tokenizer_Control::~Tokenizer_Control() {

    delete tokens;
    
}

parser::Tokenizer_Control::Tokenizer_Control(char* __srcCode) : srcCode(__srcCode) {

    tokens = new utils::LinkedList <parser::Token*>();

    parser::Token* _tk = (parser::Token*) malloc(sizeof(parser::Token));
    new (_tk) parser::Token(-1, NULL);
 
    tokens->add(
        _tk
    ); // Padding

}

void parser::Tokenizer_Control::addNewToken(parser::Token* __tk) {

    if (!__tk) return;

    if (tokens->last != NULL && __tk->id != TOKEN_IDENTIFIER && 
        (tokens->last->object->id == TOKEN_ACCESSINGVARIABLE || tokens->last->object->id == TOKEN_ACCESSINGVARIABLEPOINTER)
    ) new Tokenizer_Control_Exception("Expected identifier after trying to access field");


    tokens->add(__tk);

}

void parser::Tokenizer_Control::generateTokens() {

    while(*srcCode != 0) {
        if (*srcCode <= 32) { 
            if (tokens->last->object->id == TOKEN_ACCESSINGVARIABLE || tokens->last->object->id == TOKEN_ACCESSINGVARIABLEPOINTER)
                new Tokenizer_Control_Exception("Expected identifier after trying to access field");
            srcCode++; continue; }
        setNewToken();
    }

    if (
        parser::isIdentifierType(tokens->last->object)
    ) new Tokenizer_Control_Exception("Expected ';' token at the end"); 

    parser::Token* _fnl = (parser::Token*) malloc(sizeof(parser::Token));
    new (_fnl) parser::Token(TOKEN_END_CODE, NULL);

    tokens->add(_fnl);

    utils::DataLL <parser::Token*>* _padd = tokens->removeFrst(); // Remove padding

    delete _padd;

}

bool parser::Tokenizer_Control::handlePointerOperation(parser::Token* __tk, int __id) {

    bool _ = parser::isIdentifierType(tokens->last->object);
    if (_) __id = __id == TOKEN_POINTER ? TOKEN_MULTIPLICATION : TOKEN_BITWISEAND; 

    new (__tk) parser::Token(
        __id, NULL
    );

    return !_;

}

void parser::Tokenizer_Control::handleString(parser::Token* __tk, int __id) {

    bool _bckSlsh = 0;
    char* _inWrd = srcCode;

    do  { _bckSlsh = (*srcCode == '\\') ? 1 : 0; srcCode++; }
    while(
        (*srcCode != '"' && *srcCode != '\'') || _bckSlsh
    );
    srcCode++;

    int _s = srcCode - _inWrd;
    char* _str = (char*) malloc(_s + 1);

    for (int __ = 0; __ < _s;__++) 
        _str[__] = *(_inWrd++);

    _str[_s] = '\0';

    new (__tk) parser::Token(
        __id == TOKEN_QUOTATIONMARK ? TOKEN_STRING : TOKEN_STRING_SINGLE_QUOTE,
        _str
    );

}

void parser::Tokenizer_Control::handleDoubleSlashComment() { do srcCode++; while(*srcCode != '\n'); srcCode++; }

char* parser::Tokenizer_Control::getIdentifierData() {

    char* _inWrd = srcCode;

    do srcCode++;
    while(
        *srcCode > 32 && !parser::getTokenSymbOneChar(*srcCode)  /* Maybe need to confirm with 2 and 3 symbols TODO */
    );

    int _s = srcCode - _inWrd;

    char* _str = (char*) malloc(_s + 1);

    for (int _ = 0; _ < _s; _++) _str[_] = *(_inWrd++);

    _str[_s] = '\0';

    return _str;

}

bool parser::Tokenizer_Control::setTokenSymbol() {

    parser::Token* _tk = NULL;
    int _tkId = 0, _jmp = 0;

    if (int _tkId_temp = parser::getTokenSymbOneChar(*srcCode)) {

        _tk = (parser::Token*) malloc(sizeof(parser::Token));

        _tkId = _tkId_temp;

        _jmp = 1;

        switch (_tkId)
        {
        case TOKEN_QUOTATIONMARK: case TOKEN_SINGLE_QUOTE: handleString(_tk, _tkId); goto rtr;      
        case TOKEN_POINTER: case TOKEN_ADDRESS: if(handlePointerOperation(_tk, _tkId)) goto rtr_with_jump; break;
        default: new (_tk) parser::Token(_tkId, NULL);
        }

    }

    if (int _tkId_temp = parser::getTokenSymbTwoChars(*srcCode, *(srcCode + 1))) {

        if (!_tkId) _tk = (parser::Token*) malloc(sizeof(parser::Token));

        _tkId = _tkId_temp;

        _jmp = 2;

        if (_tkId == TOKEN_COMMENT) { handleDoubleSlashComment(); return 1; }

        new (_tk) parser::Token(_tkId, NULL);

    }

    if (int _tkId_temp = parser::getTokenSymbThreeChars(*srcCode, *(srcCode + 1), *(srcCode + 2))) {

        if (!_tkId) _tk = (parser::Token*) malloc(sizeof(parser::Token));

        _tkId = _tkId_temp;

        _jmp = 3;

        new (_tk) parser::Token(_tkId, NULL);
    }

rtr_with_jump:

    srcCode += _jmp;

rtr:

    addNewToken(_tk);

    return _tk;

}

bool parser::Tokenizer_Control::setTokenKeyWord() {

    parser::Token* _tk = NULL;

    if (int _tkId = parser::getTokenIdKeyWord(&srcCode)) { _tk = (parser::Token*) malloc(sizeof(parser::Token)); new(_tk) parser::Token(_tkId, NULL); }

    addNewToken(_tk);

    return _tk;

}

void parser::Tokenizer_Control::setTokenIdentifier() {

    parser::Token* _tk = (parser::Token*) malloc(sizeof(parser::Token));

    char* _idData = getIdentifierData();

    new (_tk) parser::Token(
        parser::getTokenIdIdetifier(_idData),
        _idData
    );

    addNewToken(_tk);

}

void parser::Tokenizer_Control::setNewToken() {

    if (setTokenSymbol());

    else if (setTokenKeyWord());

    else setTokenIdentifier();

}






