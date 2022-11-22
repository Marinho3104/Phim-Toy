#pragma once

#ifndef PARSER_HELPER_H
#define PARSER_HELPER_H

namespace parser_helper {

    /* Return size in bytes of primitives type | -1 if type is unknow
    *   Support:
    *       int
    *       ...
    *   @param _type Token id of primitve type
    */
    int getSizePrimitiveType(int);

    /* Return size in bytes of implicit value | -1 if type is unknow
    *   Support:
    *       int
    *       ...
    *   @param _type Token id of primitve type
    */
    int getSizeImplicitValue(int);

    /* Return token id corresponding to implicit value token id */
    int getTokenIdTypeFromTokenIdImplicitValue(int);

}

#endif