#include "./exception_handle.h"

#include "./parser_definitions.h"

#include "./tokenizer.h"

#include <iostream>


parser::Exception_Handle::Exception_Handle(Tokenizer_Control* __tokenizer_control, char* __description) : description(__description) {

    std::cout << "\nTokenizer Control: " << description << " " << __tokenizer_control->current_collumn << ":" << (__tokenizer_control->code - __tokenizer_control->initial_position) << "\n" << std::endl;

    exit(1);

}
