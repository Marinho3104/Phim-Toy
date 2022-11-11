#pragma once

#ifndef UTILS_LINKEDLIST_H
#define UTILS_LINKEDLIST_H

#include <iostream>

namespace utils {

    template <typename _>
    struct destructor 
        { void operator()(_&) {}; };

    template <typename _>
    struct destructor<_*> 
        { void operator()(_* tr) { tr->~_(); free(tr); }; };

    template <typename type>
    struct DataLL {

        type object;
        DataLL <type>* previous, *next;
        using destructor_t = destructor<type>;

        ~DataLL();

        DataLL(type);

    };

    template <typename type>
    struct LinkedList {

        DataLL <type>* frst, *last;
        size_t count;

        ~LinkedList();

        LinkedList();

        int add(type);

        int addFrst(type);

        void remove(DataLL <type>*);

        DataLL <type>* removeFrst();

        void join(LinkedList <type>*);

        void clean();

        type operator[](int);
        
        int getObjectPosition(type, bool (*func) (type, type));

    };

}

#endif