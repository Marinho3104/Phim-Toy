#include "./common.h"

#include <iostream>
#include <string.h>
#include <fstream>


char* utils::getFileContent(char* __path) {

    std::ifstream ifs(__path);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
        (std::istreambuf_iterator<char>()    ) );

    char* _file_content = (char*) malloc(strlen(content.c_str()) + 1);

    strcpy(_file_content, (char*) content.c_str());

    return _file_content;

}