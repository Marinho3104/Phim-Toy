/* Main goal of ast is to check if every instruction sentence makes sence */

#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct LinkedList; }


namespace parser {

    // Forward
    struct Ast_Node_Code_Block;
    struct Ast_Control;
    struct Ast_Node;
    struct Token;

    /* Type information */
    struct Type_Information {
        int token_id; // Token id of type
        //int user_defined_declaration_id; // If is token_id is identifier then this is struct declaration
        /* Can be declarated alone cause it can exist a pointer to reference so its always read the pontrLvl first and then reference lvl */
        int pointer_level; // Pointer level of type 
        int reference_level; // Reference level of type
        ~Type_Information() = default;
        /* Constructor - If something went wrong token_id is set to -1
        *   @param __tkId Token id
        *   @param __usrDecl User struct declaration TODO
        *   @param __pntrOp Pointer operations (pointer or reference)
        *   
        */
        Type_Information(int, utils::LinkedList <int>*);
        /**/
        Type_Information* generateDifferentPointersOperations(Ast_Control*);
        /* Generator */
        static Type_Information* generate(Ast_Control*);
        /* Comparates 2 types */
        bool operator==(Type_Information&);
        /* Return byte size of type holded */
        int getByteSize();
    };

    /* Storage - Holds every type and implicit value used in code */
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

    /* Ast exception */
    struct Ast_Exception {
        const char* description; // Exception description
        Ast_Exception(const char*);
    };  

    /* Ast generation control */
    struct Ast_Control {

        utils::LinkedList <Ast_Node_Code_Block*>* code_blocks; // code blocks
        utils::LinkedList <parser::Token*>* tokens_collection; // tokens collection
        Ast_Node_Code_Block* current_block;
        int current_token_position; // Keeps track of current token
        Storage* storage; // Storage

        bool debug_info;

        /* Destructor */
        ~Ast_Control();

        /* Constructor 
        *   @param __tksColl Tokens collection
        *   @param __dbgInfo Debug info set
        */
        Ast_Control(utils::LinkedList <parser::Token*>*, bool);

        /* Print debug info */
        void printDebugInfo(const char*);

        /* Generate Ast Nodes from the given tokens collection */
        void generateAst();

    };

}

#endif