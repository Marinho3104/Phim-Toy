#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Ast_Node_Code_Block;
    struct Ast_Node_Name_Space;
    struct Name_Space_Control;
    struct Type_Information;
    struct Ast_Control;
    struct Name_Space;
    struct Token;

    /* Keeps track of names declaration accors code */
    struct Name_Tracker {

        utils::LinkedList <char*>* names_declared;

        ~Name_Tracker(); Name_Tracker();

        /* Return the declaration id of given name or -1 if name is not declared */
        int getDeclarationId(char*);

        /* Adds a new name into the declaration list if is not declared 
        *   Returns true if name was declared or false if name was already declared
        */
        bool addNewName(char*, bool);

        static int addToCorrectNameTracker(Ast_Control*);

        static int getDeclarationId(Ast_Control*, char*);

        static int isDeclarationIdGlobal(Ast_Control*, char*);

    };

    /* Represent a type */
    struct Type_Information {
        int token_id, user_defined_declaration_id; // Token id representing the type | if token_id == identifier means is a user defined type (a.k.a struct)
        int pointer_level, reference_level; // Pointer ans reference level of type
        Name_Space* name_space; // if null is current name space

        ~Type_Information(); Type_Information();

        Type_Information(int, int, utils::LinkedList <int>*, Name_Space*);

        static Type_Information* generate(Ast_Control*, Name_Space*);

        static Type_Information* generate(Ast_Control*, Type_Information*);

        bool operator==(Type_Information&);

        int getByteSize();

    };

    /* Keeps track of every declaration occur in a name space */
    struct Name_Space {

        Name_Space_Control* name_space_control;
        utils::LinkedList <char*>* name_space_scope;
        Name_Tracker* name_tracker;
        int off_count;

        ~Name_Space(); Name_Space(Name_Space_Control*, utils::LinkedList <char*>*);

        void offCountSet();

        bool addNewName(char*);

        int getDeclarationId(char*);

        static Name_Space* checkIfNameSpace(Ast_Control*, int*);

        static Name_Space* getNameSpaceFromStruct(Ast_Control*, utils::LinkedList <char*>*, int*);

        static Name_Space* getNameSpace(Ast_Control*, bool, int*);

        static Name_Space* getNameSpace(Ast_Control*);

        void print();

    };

    /* Keeps track of every name space created in code
    *   Name space with 0 names in scope is global one
    */
    struct Name_Space_Control {

        utils::LinkedList <Name_Space*>* name_space_declarations;
        Name_Space* global_name_space;

        ~Name_Space_Control(); Name_Space_Control();

        Name_Space* getNameSpaceDefinition(utils::LinkedList <char*>*);

        Name_Space* getPreviousNameSpace(Name_Space*);

        Name_Space* getNameSpace(utils::LinkedList <char*>*);

        Name_Space* getNameSpaceStruct();

    };

    /* Store every implicit value */
    struct Storage {

        utils::LinkedList <char*>* implicit_values;

        ~Storage(); Storage();

        /* Add new value into Linked List
        *   If value already exists dont add
        *   @return Pos of value given in Linked List 
        */
        int addNewValue(char*, bool);

    };

    struct Ast_Execption { const char* description; Ast_Execption(const char*); };

    struct Ast_Control {

        utils::LinkedList <Ast_Node_Name_Space*>* name_spaces; // all code is here
        utils::LinkedList <Token*>* tokens_collection;
        Name_Space* current_name_space, *struct_name_space; // Currnet name space in use
        Name_Space_Control* name_space_control;
        Ast_Node_Code_Block* current_code_block; // Current code block in use
        int current_token_position; // Keeps track of current token position
        Storage* storage;

        bool debug_info;

        Name_Space* current_name_space_saved, *struct_name_space_saved;
        Ast_Node_Code_Block* current_code_block_saved;

        ~Ast_Control(); Ast_Control(utils::LinkedList <Token*>*, bool);

        void printDebugInfo(const char*);

        parser::Token* getToken(int);

        void saveState();

        void setPreviousSavedState();

        void generate();

    };

}

#endif