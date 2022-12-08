#pragma once

#ifndef PARSER_EXCEPTION_HANDLE_H
#define PARSER_EXCEPTION_HANDLE_H

namespace parser {

    // Forward
    struct Tokenizer_Control;

    struct Exception_Handle {

        char* description;

        Exception_Handle(Tokenizer_Control*, char*);

    };

}

#endif