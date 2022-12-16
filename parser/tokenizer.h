#pragma once

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

    // Forward
    struct Tokenizer_Control;
    struct Token;

    extern Tokenizer_Control* tokenizer_control;

    struct Tokenizer_Control {

        utils::Linked_List <Token*>* tokens_collection;
        bool debug_info;
        char* code;

        char* initial_position;
        int current_collumn;

        ~Tokenizer_Control(); Tokenizer_Control(char*, bool);

        void printDebugInfo();

        bool isInRange(int);

        void addToken(parser::Token*);

        void generate();

        void setNewToken();

        bool setIfIsTokenSymbol();

        bool setIfIsTokenKeyWord();

        void setIfIsTokenIdentifier();

        void handleString(parser::Token*);

        void handlePointerOperation(parser::Token*);

        void handleDoubleSlashComment();

        char* getIdentifierData();

    };

    void setUpTokenizer(char*, bool);

    void cleanTokenizer();

}

#endif