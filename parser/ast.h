#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Ast_Node_Code_Block;
    struct Ast_Node_Name_Space;
    struct Name_Space_Control;
    struct Ast_Control;
    struct Name_Space;
    struct Token;

    /* Represent a type */
    struct Type_Information {
        int token_id, user_defined_declaration_id; // Token id representing the type | if token_id == identifier means is a user defined type (a.k.a struct)
        int pointer_level, reference_level; // Pointer ans reference level of type
        Name_Space* name_space; // if null is current name space
        Ast_Control* ast_control;

        ~Type_Information(); Type_Information();

        Type_Information(Ast_Control*, int, int, utils::LinkedList <int>*, Name_Space*);

        static Type_Information* generate(Ast_Control*, Name_Space*);

        static Type_Information* generate(Ast_Control*, Type_Information*);

        bool operator==(Type_Information&);

        int getByteSize();

        Type_Information* getCopy();

    };

    struct Name_Tracker {

        utils::LinkedList <char*>* names_declared;
        utils::LinkedList <int>* names_declarations_id;
        int* off;

        ~Name_Tracker(); Name_Tracker(int*);

        int getDeclarationId(char*);

        bool addNewName(char*);

    };

    struct Name_Space {

        Name_Space_Control* name_space_control;
        utils::LinkedList <char*>* scope;
        Name_Tracker* name_tracker;

        ~Name_Space(); Name_Space(Name_Space_Control*, utils::LinkedList <char*>*);

        void updateOff(int*);

        bool addNewName(char*);

        int getDeclarationId(char*);

        void printScope();

    };

    struct Name_Space_Control {

        utils::LinkedList <parser::Name_Space*>* name_spaces;
        int declarations_count;

        ~Name_Space_Control(); Name_Space_Control();

        bool addNameSpace(Name_Space*);

        Name_Space* getNameSpace(utils::LinkedList <char*>*);

        Name_Space* getNameSpaceSpecific(utils::LinkedList <char*>*);

        Name_Space* getNameSpaceOrAdd(utils::LinkedList <char*>*);

        Name_Space* getPreviousNameSpace(utils::LinkedList <char*>*);



    };

    struct Ast_Exception { const char* description; Ast_Exception(const char*); };

    struct Ast_Control {

        utils::LinkedList <Ast_Node_Name_Space*>* nodes_name_spaces;

        utils::LinkedList <Name_Space*>* name_spaces_saved, *struct_name_spaces_saved;
        utils::LinkedList <Ast_Node_Code_Block*>* code_blocks_saved;

        utils::LinkedList <Token*>* tokens_collection;
        Name_Space_Control* name_space_control;
        int current_token_position;
        
        Ast_Node_Code_Block* current_code_block;
        Name_Space* current_name_space, *current_struct_name_space;

        bool debug_info;

        ~Ast_Control(); Ast_Control(utils::LinkedList <Token*>*, bool);

        void saveState();

        void setLastSavedState();

        void printDebugInfo(const char*);

        parser::Token* getToken(int);

        void generate();

    };

}

#endif



