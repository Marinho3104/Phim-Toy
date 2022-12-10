#include "./exception_handle.h"

#include "./parser_definitions.h"
#include "./token.h"

#include "./tokenizer.h"
#include "./ast.h"

#include <iostream>

parser::Exception_Handle::Exception_Handle(const char* __description) : description(__description) {

    std::cout << "Exception: " << description << std::endl;

    exit(1);

}

parser::Exception_Handle::Exception_Handle(Tokenizer_Control* __tokenizer_control, const char* __description) : description(__description) {

    std::cout << "\nTokenizer Control: " << description << " " << __tokenizer_control->current_collumn << ":" << (__tokenizer_control->code - __tokenizer_control->initial_position) << "\n" << std::endl;

    exit(1);

}

parser::Exception_Handle::Exception_Handle(Ast_Control* __ast_control, Token* __token, const char* __description) : description(__description) {

    std::cout << "\nAst Control: " << description << " " << __token->collumn_position << ":" << __token->line_position << "\n" << std::endl;

    exit(1);

}

