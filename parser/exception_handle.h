#pragma once

#ifndef PARSER_EXCEPTION_HANDLE_H
#define PARSER_EXCEPTION_HANDLE_H

namespace parser {

    // Forward
    struct Tokenizer_Control;
    struct Ast_Control;
    struct Token;

    struct Exception_Handle {

        const char* description;

        Exception_Handle(const char*);

        Exception_Handle(Tokenizer_Control*, const char*);

        Exception_Handle(Ast_Control*, Token*, const char*);

    };

}

#endif