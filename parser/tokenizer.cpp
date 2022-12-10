#include "./tokenizer.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./exception_handle.h"
#include "./token.h"

#include <iostream>
#include <string.h>

parser::Tokenizer_Control::~Tokenizer_Control() { delete tokens_collection; }

parser::Tokenizer_Control::Tokenizer_Control(char* __code, bool __debug_info) : code(__code), debug_info(__debug_info), initial_position(__code), current_collumn(0) { tokens_collection = new utils::Linked_List <Token*>(); }
 
void parser::Tokenizer_Control::printDebugInfo() { if (debug_info) std::cout << "\tTokenizer Control - Debug Info: Token added -> " << tokens_collection->last->object->id << std::endl; }

bool parser::Tokenizer_Control::isInRange(int __off) {

    for (int _ = 0; _ < __off + 1; _++) if (!*(code + _)) return 0;

    return 1;

}

void parser::Tokenizer_Control::addToken(parser::Token* __to_add) { 
    if (!__to_add) return; 
    __to_add->collumn_position = current_collumn; __to_add->line_position = code - initial_position - 1;  
    tokens_collection->add(__to_add); 
}

void parser::Tokenizer_Control::generate() {

    if (debug_info) std::cout << "\n\t\t--- Tokenizer Control ---\n" << std::endl;

    while(*code) {

        if (*code <= 32) { code++; if (*(code - 1) == '\n') { current_collumn++; initial_position = code; }  continue; }

        setNewToken();

    }

}

bool parser::Tokenizer_Control::setIfIsTokenSymbol() {

    int _token_id_temp, _token_id, _jmp = 0; 
    parser::Token* _token = NULL;

    if (
        isInRange(0) && (_token_id_temp = parser::getTokenSymbOneChar(*code))
    ) {

        _token = (parser::Token*) malloc(sizeof(parser::Token));

        _token_id = _token_id_temp;

        _jmp = 1;

        new (_token) parser::Token(_token_id, NULL);

        switch (_token_id)
        {
        case TOKEN_QUOTATIONMARK: case TOKEN_SINGLE_QUOTE: handleString(_token); goto rtr;
        case TOKEN_POINTER: case TOKEN_ADDRESS: handlePointerOperation(_token); goto rtr_with_jump;      
        default: break;
        }

    }

    if (
        isInRange(1) && (_token_id_temp = parser::getTokenSymbTwoChars(*code, *(code + 1)))
    ) {

        if (!_token) _token = (parser::Token*) malloc(sizeof(parser::Token));

        _token_id = _token_id_temp;

        _jmp = 2;

        switch (_token_id)
        {
        case TOKEN_COMMENT: free(_token); handleDoubleSlashComment(); return 1;
        default: break;
        }

        new (_token) parser::Token(_token_id, NULL);

    }

    if (
        isInRange(2) && (_token_id_temp = parser::getTokenSymbThreeChars(*code, *(code + 1), *(code + 2)))
    ) {

        if (!_token) _token = (parser::Token*) malloc(sizeof(parser::Token));

        _token_id = _token_id_temp;

        _jmp = 3;

        new (_token) parser::Token(_token_id, NULL);

    }

rtr_with_jump:

    code += _jmp;

rtr:

    addToken(_token);

    if (_token) printDebugInfo();

    return _token;

}

bool parser::Tokenizer_Control::setIfIsTokenKeyWord() {

    parser::Token* _token = NULL;
    int _token_id;

    if (_token_id = parser::getTokenIdKeyWord(&code)) { // Miss check if has space for check
        _token = (parser::Token*) malloc(sizeof(parser::Token)); 
        new(_token) parser::Token(_token_id, NULL); 
    }

    addToken(_token);

    if (_token) printDebugInfo();
    
    return _token;

}

void parser::Tokenizer_Control::setIfIsTokenIdentifier() {

    parser::Token* _token = (parser::Token*) malloc(sizeof(parser::Token));

    char* _idData = getIdentifierData();

    new (_token) parser::Token(
        parser::getTokenIdImplicitValue(_idData),
        _idData
    );

    addToken(_token);

    if (_token) printDebugInfo();

}

void parser::Tokenizer_Control::setNewToken() {

    if (setIfIsTokenSymbol());

    else if (setIfIsTokenKeyWord());

    else setIfIsTokenIdentifier();

}

void parser::Tokenizer_Control::handleString(parser::Token* __token) {

    int _id = __token->id;

    char* _incitial_word = code;
    bool _back_slash = 0;
    int _string_size;

    do {

        if (!isInRange(0)) new Exception_Handle(this, "Expected token '\"' at the end of string");
        
        code++;

        _back_slash = *code == '\\' ? 1 : 0;
        
    }

    while(
        (*code != '"' && *code != '\'') || _back_slash
    );

    if (!isInRange(0)) new Exception_Handle(this, "Expected token '\"' at the end of string");
    code++;

    _string_size = code - _incitial_word;
 
    char* _string = (char*) malloc(_string_size + 1);

    strncpy(_string, _incitial_word, _string_size);

    _string[_string_size] = '\0';

    new (__token) parser::Token(
        _id == TOKEN_QUOTATIONMARK ? TOKEN_STRING : TOKEN_STRING_SINGLE_QUOTE,
        _string
    );

}

void parser::Tokenizer_Control::handlePointerOperation(parser::Token* __token) {

    if (!parser::isPrimitiveTokenId(__token->id)) __token->id = __token->id == TOKEN_POINTER ? TOKEN_MULTIPLICATION : TOKEN_BITWISEAND;

}

void parser::Tokenizer_Control::handleDoubleSlashComment() { do { if (!isInRange(0)) break; code++; } while(*code != '\n'); if (isInRange(0)) code++; }

char* parser::Tokenizer_Control::getIdentifierData() {

    char* _inicial_word = code;

    do { if (!isInRange(0)) break;  code++; }
    while(
        *code > 32 && !parser::getTokenSymbOneChar(*code)  /* Maybe need to confirm with 2 and 3 symbols TODO */
    );

    int _string_size = code - _inicial_word;

    char* _string = (char*) malloc(_string_size + 1);

    strncpy(_string, _inicial_word, _string_size);

    _string[_string_size] = '\0';

    return _string;

}


