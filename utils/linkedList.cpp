#include "./linkedList.h"

// #include "./../objects/phim_object.h"

// #include "./../parser/compiler.h" // Compiler Code Block
#include "./../byteCode/byteCode.h" // Byte Code
#include "./../parser/ast_nodes.h" // Ast Nodes
#include "./../parser/token.h" // Tokens Id
#include "./../parser/ast.h" // Ast Contrl | TypeInformation
// #include "./../parser/compiled_byte_code.h" // Compiler byte code

#include <type_traits>
#include <iostream>
#include <string.h>

template <typename type>
utils::DataLL<type>::~DataLL() { 
    
    if (destroy_content) destructor_t()(object); 
    else {
        
        destroy_content = 1;
    
        if (next) next->destroy_content = 0;

    }
    
    if (next) delete next; 

}

template <typename type>
utils::DataLL<type>::DataLL(type _) : object(_), next(NULL), previous(NULL), destroy_content(1) {}

/* Separator */

template <typename type>
utils::LinkedList<type>::~LinkedList() { if (frst && !destroy_content) frst->destroy_content = 0;  delete frst; }

template <typename type>
utils::LinkedList<type>::LinkedList() : frst(NULL), last(NULL), count(0), destroy_content(1) {}

template <typename type>
int utils::LinkedList<type>::add(type _) {

    utils::DataLL<type>* _toAdd = new utils::DataLL(_);

    if (!frst) { frst = _toAdd; last = frst; }

    else if (frst == last) { frst->next = _toAdd; _toAdd->previous = frst; last = _toAdd; }

    else { last->next = _toAdd; _toAdd->previous = last; last = _toAdd; }

    return count++;

}

template <typename type>
int utils::LinkedList<type>::addFrst(type _) {

    utils::DataLL<type>* _toAdd = new utils::DataLL(_);

    if (!frst) { frst = _toAdd; last = frst; }

    else { _toAdd->next = frst; frst->previous = _toAdd; frst = _toAdd; }

    ++count;

    return 0;

}

template <typename type>
void utils::LinkedList<type>::remove(DataLL <type>* _) {

    if (_ == frst) {

        if (_ == last) {

            frst = NULL;
            last = NULL;

        }

        else frst = frst->next;

    }

    else if (_ == last) {

        last->previous->next = NULL;

        last = last->previous;

    }

    else {

        _->previous->next = _->next;
        _->next->previous = _->previous;

    }

    --count;

}

template <typename type>
utils::DataLL <type>* utils::LinkedList<type>::removeFrst() {

    utils::DataLL <type>* _ = frst;

    if (frst) {

        if (frst == last) { frst = NULL; last = NULL; }

        else { frst = frst->next; frst->previous = NULL; }
    
        --count;

    }

    _->next = NULL;

    return _;

}

template <typename type>
bool utils::LinkedList<type>::join(utils::LinkedList <type>* _toJoin) {

    if (!_toJoin) return false;
    if (!_toJoin->count) return false;
    
    int _  = 0;

    for (; _ < _toJoin->count; _++) add( (*_toJoin)[_] );

    return true;

}

template <typename type>
void utils::LinkedList<type>::clean() {

    delete frst;

    frst = NULL;
    last = NULL;
    count = 0;

}

template <typename type>
type utils::LinkedList<type>::operator[](int _indx) {

    if (_indx >= count) return NULL;

    utils::DataLL <type>* _rtr = frst;

    for (int _ = 0; _ < _indx; _++) _rtr = _rtr->next;

    return _rtr->object;

}

template <typename type>
utils::DataLL<type>* utils::LinkedList<type>::getDataLL(int _indx) {

    if (_indx >= count) return NULL;

    utils::DataLL <type>* _rtr = frst;

    for (int _ = 0; _ < _indx; _++) _rtr = _rtr->next;

    return _rtr;

}

template <typename type>
int utils::LinkedList<type>::getObjectPosition(type _toCmp, bool (*func) (type, type)) {

    for (int _ = 0; _ < count; _++)

        if (func(operator[](_), _toCmp)) return _;

    return -1;

}

template <>
int utils::LinkedList<int>::getObjectPosition(int _toCmp, bool (*func) (int, int)) {

    for (int _ = 0; _ < count; _++)

        if (operator[](_) == _toCmp) return _;

    return -1;

}

template <>
int utils::LinkedList<char*>::getObjectPosition(char* _toCmp, bool (*func) (char*, char*)) {

    for (int _ = 0; _ < count; _++)
        if (strcmp(operator[](_), _toCmp) == 0) return _;

    return -1;

}


// template class utils::DataLL <parser::Ast_Node_Function_Declaration*>;
// template class utils::DataLL <parser::Ast_Node_Variable_Declaration*>;
// template class utils::DataLL <parser::Ast_Node_Struct_Declaration*>;
// template class utils::DataLL <parser::Compiled_Code_Block*>;
// template class utils::DataLL <parser::Compiler_Code_Block*>;
// template class utils::DataLL <parser::Ast_Node_Code_Block*>;
// template class utils::DataLL <parser::Type_Information*>;


// template class utils::DataLL <objects::Phim_Object*>;

// template class utils::DataLL <parser::Ast_Node_Expression*>;
template class utils::DataLL <parser::Ast_Node_Name_Space*>;
template class utils::DataLL <byte_code::Byte_Code*>;
template class utils::DataLL <parser::Name_Space*>;
template class utils::DataLL <parser::Ast_Node*>;
template class utils::DataLL <parser::Token*>;
template class utils::DataLL <char*>;
template class utils::DataLL <int>;

// template class utils::LinkedList <parser::Ast_Node_Function_Declaration*>;
// template class utils::LinkedList <parser::Ast_Node_Variable_Declaration*>;
// template class utils::LinkedList <parser::Ast_Node_Struct_Declaration*>;
// template class utils::LinkedList <parser::Compiled_Code_Block*>;
// template class utils::LinkedList <parser::Compiler_Code_Block*>;
// template class utils::LinkedList <parser::Ast_Node_Code_Block*>;
// template class utils::LinkedList <parser::Type_Information*>;
// template class utils::LinkedList <objects::Phim_Object*>;

// template class utils::LinkedList <parser::Ast_Node_Expression*>;
template class utils::LinkedList <parser::Ast_Node_Name_Space*>;
template class utils::LinkedList <byte_code::Byte_Code*>;
template class utils::LinkedList <parser::Name_Space*>;
template class utils::LinkedList <parser::Ast_Node*>;
template class utils::LinkedList <parser::Token*>;
template class utils::LinkedList <char*>;
template class utils::LinkedList <int>;








