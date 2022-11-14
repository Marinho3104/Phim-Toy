#include "./optimizer.h"

#include "./../utils/linkedList.h"

#include <iostream>

void parser::optimizerNames(utils::LinkedList <char*>* _toOp) {

    char _crrntChr = 1;
    utils::DataLL<char*>* _crrntDataLL; 

    for (int _ = 0; _ < _toOp->count; _++) {
        
        _crrntDataLL = _toOp->getDataLL(_);

        free(_crrntDataLL->object);

        _crrntDataLL->object = (char*) malloc(sizeof(char));

        *_crrntDataLL->object = _crrntChr++; 

    }


}

