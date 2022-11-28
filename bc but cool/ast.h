/* Main goal of ast is to check if every instruction sentence makes sence */

#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Ast_Node_Code_Block;
    struct Name_Space_Control;
    struct Ast_Control;
    struct Token;

    /* Type information */
    struct Type_Information {
        // Ast_Node_Struct_Declaration* user_defined_declaration; // If is token_id is identifier then this is struct declaration
        int token_id, user_defined_declaration_id; // Token id of type
        /* Can be declarated alone cause it can exist a pointer to reference so its always read the pontrLvl first and then reference lvl */
        int pointer_level; // Pointer level of type 
        int reference_level; // Reference level of type
        ~Type_Information() = default;
        Type_Information() = default;
        /* Constructor - If something went wrong token_id is set to -1
        *   @param __tkId Token id
        *   @param __usrDecl User struct declaration TODO
        *   @param __pntrOp Pointer operations (pointer or reference)
        *   
        */
        Type_Information(int, int, utils::LinkedList <int>*);
        /**/
        Type_Information* generateDifferentPointersOperations(Ast_Control*);
        /* Generator */
        static Type_Information* generate(Ast_Control*);
        /* Comparates 2 types */
        bool operator==(Type_Information&);
        /* Return byte size of type holded */
        int getByteSize();
    };

    /* Keeps track of names used in code as well as their declarations id*/
    struct Names_Tracker {

        utils::LinkedList <char*>* names; // Names declared

        ~Names_Tracker(); Names_Tracker();

        /* Return the declaration id of given name or -1 if name is not declared */
        int getDeclarationId(char*);

        /* Adds a new name into the declaration list if is not declared 
        *   Returns true if name was declared or false if name was already declared
        */
        bool addNewName(char*, bool);

    };

    /* Keeps track of all declarations, names occur inside a name space*/
    struct Name_Space {

        utils::LinkedList <char*>* name_space_scope;
        Name_Space_Control* name_space_control;
        Ast_Node_Code_Block* declarations;
        Names_Tracker* names_tracker;

        ~Name_Space(); Name_Space(utils::LinkedList <char*>*, Name_Space_Control*);

        int getDeclarationId(char*);

        void generateCodeBlock(Ast_Control*);

    };

    /* Keeps track of all name spaces declared */
    struct Name_Space_Control {

        utils::LinkedList <Name_Space*>* name_spaces;
        Name_Space* global_name_space;

        ~Name_Space_Control(); Name_Space_Control();

        Name_Space* isNameSpaceDefined(utils::LinkedList <char*>*);

        Name_Space* getPreviousNameSpace(Name_Space*);

        Name_Space* getNameSpace(utils::LinkedList <char*>*);

    };

    /* Storage - Holds every implicit value used in code */
    struct Storage {

        utils::LinkedList <char*>* implicit_values; // Implicit values used

        /* Destructor */
        ~Storage();
        /* Constructor */
        Storage();
        /* Add new value into Linked List
        *   If value already exists dont add
        *   @return Pos of value given in Linked List 
        */
        int addNewValue(char*, bool);

    };

    struct Ast_Exception { char* description; Ast_Exception(char*); };

    struct Ast_Control {

        utils::LinkedList <parser::Token*>* tokens_collection; // tokens collection
        Ast_Node_Code_Block* current_code_block; 
        Name_Space_Control* name_space_control;
        int current_token_position;
        Storage* storage;

        bool debug_info;

        ~Ast_Control(); Ast_Control(utils::LinkedList <parser::Token*>*, bool);

        void printDebugInfo(const char*);

        parser::Token* getToken(int);

        void generateAst();

    };

}

#endif