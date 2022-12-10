#pragma once

#ifndef UTILS_LINKED_LIST_H
#define UTILS_LINKED_LIST_H

#include <iostream>

namespace utils {

    template <typename type>
    struct Destructor 
        { void operator()(type&) {} };
    
    template <typename type>
    struct Destructor <type*>
        { void operator()(type* __to_destroy) { __to_destroy->~type(); free(__to_destroy); } };

    template <typename type>
    struct Data_Linked_List {

        type object;
        Data_Linked_List <type>* previous, *next;
        bool destroy_content;
        using destructor = Destructor <type>;

        ~Data_Linked_List(); Data_Linked_List(type);

        void linkAfter(Data_Linked_List <type>*);
        
        void linkBefore(Data_Linked_List <type>*);

    };

    template <typename type>
    struct Linked_List {

        Data_Linked_List <type>* first, *last;
        bool destroy_content;
        int count;

        ~Linked_List(); Linked_List();

        bool operator==(Linked_List <type>*);

        bool operator!=(Linked_List <type>*);

        type operator[](int);

        Data_Linked_List <type>* getDataLinkedList(int);

        void setCount();

        void clean();

        void printContent();

        int add(type); Data_Linked_List <type>* remove(int);

        void insert(type, int); void remove(Data_Linked_List <type>*);

        bool join(Linked_List <type>*);

        bool join(Linked_List <type>*, int);

        int getPosition(type, bool (*func) (type, type));


    };

}

#endif
