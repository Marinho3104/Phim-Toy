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
        bool destroy_content;
        using destructor_t = destructor<type>;

        ~DataLL();

        DataLL(type);

    };

    template <typename type>
    struct LinkedList {

        DataLL <type>* frst, *last;
        bool destroy_content;
        size_t count;

        ~LinkedList();

        void destroyNoTouch();

        LinkedList();

        int add(type);

        int addFrst(type);

        void remove(DataLL <type>*);

        DataLL <type>* removeFrst();

        bool join(LinkedList <type>*);

        void clean();

        type operator[](int);

        DataLL<type>* getDataLL(int);
        
        int getObjectPosition(type, bool (*func) (type, type));

    };

}

#endif