#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

namespace parser { struct Ast_Control; }

namespace parser_helper {

    void setPntrRfrnLevel(parser::Ast_Control*, int*, int*);

}

#endif