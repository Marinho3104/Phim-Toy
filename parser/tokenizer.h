#pragma once

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Token;

    struct Tokenizer_Control_Exception {
        char* description; // Exception Description
        Tokenizer_Control_Exception(char*);
    };

    struct Tokenizer_Control {

        utils::LinkedList <parser::Token*>* tokens;
        bool is_token_last_special_character;
        char* srcCode;

        ~Tokenizer_Control();

        Tokenizer_Control(char*);

        void addNewToken(parser::Token*);

        void generateTokens();

        bool handlePointerOperation(parser::Token*, int);

        void handleString(parser::Token*, int);

        void handleDoubleSlashComment();

        char* getIdentifierData();

        bool setTokenSymbol();

        bool setTokenKeyWord();

        void setTokenIdentifier();

        void setNewToken();

    };

}

#endif