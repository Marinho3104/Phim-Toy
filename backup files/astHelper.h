#pragma once

#ifndef AST_HELPER_H
#define AST_HELPER_H

namespace utils { template <typename> struct LinkedList; }
namespace token { struct Token; }
namespace parser { struct Ast_Control; }

namespace parser_helper {

    void setPntrRfrnLevel(parser::Ast_Control*, int*, int*);

}


#endif