#include "./linkedList.h"

#include "./../parser/ast_nodes.h"
#include "./../parser/token.h"
#include "./../parser/ast.h"

#include "./../byte_code/byte_code.h"

#include <iostream>
#include <string.h>

template <typename type>
utils::Data_Linked_List<type>::~Data_Linked_List() {

    if (destroy_content) destructor()(object);
    else 
        if (next) next->destroy_content = 0;

    if (next) delete next;

}

template <typename type>
utils::Data_Linked_List<type>::Data_Linked_List(type __obj) : object(__obj), previous(NULL), next(NULL), destroy_content(1) {}

template <typename type>
void utils::Data_Linked_List<type>::linkAfter(Data_Linked_List <type>* __to_link) { next = __to_link; __to_link->previous = this; }

template <typename type>
void utils::Data_Linked_List<type>::linkBefore(Data_Linked_List <type>* __to_link) { previous = __to_link; __to_link->next = this; }


template <typename type>
utils::Linked_List <type>::~Linked_List() { if (!first) return; first->destroy_content = destroy_content; delete first; }

template <typename type>
utils::Linked_List <type>::Linked_List() : first(NULL), last(NULL), count(0), destroy_content(1) {}

template <typename type>
utils::Data_Linked_List <type>* utils::Linked_List <type>::getDataLinkedList(int __index) {

    if (__index < 0 || !count) return NULL; utils::Data_Linked_List <type>* _data_ll = first;

    for (int _ = 0; _ < __index; _++) 
        if (_data_ll->next) _data_ll = _data_ll->next; 
        else break;

    return _data_ll;

}

template <typename type>
bool utils::Linked_List <type>::operator==(Linked_List <type>* __to_compare) {

    if (count != __to_compare->count) return 0;

    for (int _ = 0; _ < __to_compare->count; _++)

        if (operator[](_) != __to_compare->operator[](_)) return 0;

    return 1;

}

template <>
bool utils::Linked_List <char*>::operator==(Linked_List <char*>* __to_compare) {

    if (count != __to_compare->count) return 0;

    for (int _ = 0; _ < __to_compare->count; _++) if (strcmp(operator[](_), __to_compare->operator[](_))) return 0;

    return 1;

}

template <typename type>
bool utils::Linked_List <type>::operator!=(Linked_List <type>* __to_compare) { return !operator==(__to_compare); }

template <typename type>
type utils::Linked_List <type>::operator[](int __index) { return __index < 0 ? 0 : getDataLinkedList(__index)->object; }

template <typename type>
void utils::Linked_List <type>::setCount() { Data_Linked_List <type>* _current = first; count = 0; while(_current) { ++count; _current = _current->next; } }

template <typename type>
void utils::Linked_List <type>::clean() { this->~Linked_List(); first = NULL; last = NULL; count = 0; }

template <typename type>
void utils::Linked_List <type>::printContent() {

    std::cout << "[ ";

    for (int _ = 0; _ < count; _ ++) std::cout << operator[](_) << " ";

    std::cout << "]" << std::endl;

}

template <typename type>
int utils::Linked_List <type>::add(type __to_add) { insert(__to_add, count); return count - 1; }

template <typename type>
utils::Data_Linked_List <type>* utils::Linked_List <type>::remove(int __index) {
    
    utils::Data_Linked_List <type>* _removed_data_linked_list = getDataLinkedList(__index); remove(_removed_data_linked_list);

    return _removed_data_linked_list;
      
}

template <typename type>
void utils::Linked_List <type>::insert(type __to_add, int __index) {

    Data_Linked_List <type>* _data_ll_before = getDataLinkedList(__index - 1), *_data_ll_after, *_data_ll = new Data_Linked_List<type>(__to_add);

    if (!first) { first = _data_ll; last = first; }

    else if (!_data_ll_before) { first->linkBefore(_data_ll); first = _data_ll; }

    else if (_data_ll_before == last) { last->linkAfter(_data_ll); last = _data_ll; }

    else { _data_ll_after = getDataLinkedList(__index); _data_ll_before->linkAfter(_data_ll); _data_ll_after->linkBefore(_data_ll); }

    ++count;

}

template <typename type>
void utils::Linked_List <type>::remove(Data_Linked_List <type>* __to_remove) { /* Given argument needs to be in array */

    if (!__to_remove) return;

    if (first == last)  { first = NULL; last = NULL; }

    else if (__to_remove == first) { first->next->previous = NULL; first = first->next; }

    else if (__to_remove == last) { last->previous->next = NULL; last = last->previous; }

    else { __to_remove->previous->linkAfter(__to_remove->next); __to_remove->next->linkBefore(__to_remove->previous); }

    __to_remove->next = NULL;

    --count;

}

template <typename type>
bool utils::Linked_List <type>::join(Linked_List <type>* __to_join) {

    if (!__to_join->count) return 0;

    for (int _ = 0; _ < __to_join->count; _++) add(__to_join->operator[](_));

    return 1;

}

template <typename type>
bool utils::Linked_List <type>::join(Linked_List <type>* __to_join, int __index) {

    if (!__to_join->count) return 0;

    for (int _ = 0; _ < __to_join->count; _++, __index++) insert(__to_join->operator[](_), __index);

    return 1;

}

template <typename type>
int utils::Linked_List <type>::getPosition(type _to_compare, bool (*func) (type, type)) {

    for (int _ = 0; _ < count; _++)

        if (func(operator[](_), _to_compare)) return _;

    return -1;

}

template <>
int utils::Linked_List <int>::getPosition(int _to_compare, bool (*func) (int, int)) {

    for (int _ = 0; _ < count; _++)

        if (operator[](_) == _to_compare) return _;

    return -1;

}

template <>
int utils::Linked_List <char*>::getPosition(char* _to_compare, bool (*func) (char*, char*)) {

    for (int _ = 0; _ < count; _++)

        if (!strcmp(operator[](_), _to_compare)) return _;

    return -1;

}



template class utils::Linked_List <byte_code::Byte_Code*>;
template class utils::Linked_List <byte_code::Byte_Code_Block*>;
template class utils::Linked_List <byte_code::Compiled_Byte_Code*>;

template class utils::Data_Linked_List <byte_code::Byte_Code*>;
template class utils::Data_Linked_List <byte_code::Byte_Code_Block*>;
template class utils::Data_Linked_List <byte_code::Compiled_Byte_Code*>;


template class utils::Linked_List <parser::Ast_Node_Function_Declaration*>;
template class utils::Linked_List <parser::Ast_Node_Variable_Declaration*>;
template class utils::Linked_List <parser::Ast_Node_Struct_Declaration*>;
template class utils::Linked_List <parser::Ast_Node_Expression*>;
template class utils::Linked_List <parser::Ast_Node_Code_Block*>;
template class utils::Linked_List <parser::Ast_Node_Name_Space*>;
template class utils::Linked_List <parser::Name_Space*>;
template class utils::Linked_List <parser::Ast_Node*>;
template class utils::Linked_List <parser::Token*>;
template class utils::Linked_List <char*>;
template class utils::Linked_List <int>;

template class utils::Data_Linked_List <parser::Ast_Node_Function_Declaration*>;
template class utils::Data_Linked_List <parser::Ast_Node_Variable_Declaration*>;
template class utils::Data_Linked_List <parser::Ast_Node_Struct_Declaration*>;
template class utils::Data_Linked_List <parser::Ast_Node_Expression*>;
template class utils::Data_Linked_List <parser::Ast_Node_Code_Block*>;
template class utils::Data_Linked_List <parser::Ast_Node_Name_Space*>;
template class utils::Data_Linked_List <parser::Name_Space*>;
template class utils::Data_Linked_List <parser::Ast_Node*>;
template class utils::Data_Linked_List <parser::Token*>;
template class utils::Data_Linked_List <char*>;
template class utils::Data_Linked_List <int>;




