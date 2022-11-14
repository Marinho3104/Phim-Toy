#pragma once

#ifndef PARSER_OPTIMIZER_H
#define PARSER_OPTIMIZER_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    /* Optimize names to be short as possible */
    void optimizerNames(utils::LinkedList <char*>*);

}

#endif