#include "./built_in.h"

#include "./../parser/tokenizer.h"
#include "./../utils/common.h"

#include <iostream>

compiler::Built_In::~Built_In() {}

compiler::Built_In::Built_In() { generate(); }

void compiler::Built_In::generate() {

    char* built_in_content = utils::getFileContent("./../built_in/byte.ph");

    parser::Tokenizer_Control* tokenizer_control = new parser::Tokenizer_Control(built_in_content, 1);

    tokenizer_control->generate();

    delete tokenizer_control;

    free(built_in_content);

}


