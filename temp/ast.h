#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

    // Forward
    struct Ast_Node_Name_Space;
    struct Ast_Node_Code_Block;
    struct Name_Space_Control;
    struct Tokenizer_Control;
    struct Token;

    struct Name_Tracker {

        utils::Linked_List <char*>* names;
        utils::Linked_List <int>* declaration_ids;

        int* off;

        ~Name_Tracker(); Name_Tracker();

        bool addName(char*);

        int getDeclarationId(char*);

    };

    struct Name_Space {

        Name_Space_Control* name_space_control;
        utils::Linked_List <char*>* scope;
        Name_Tracker* name_tracker;

        ~Name_Space(); Name_Space(Name_Space_Control*, utils::Linked_List <char*>*);

        void updateOff(int*);

        bool addName(char*);

        int getDeclarationId(char*);

        void printScope();

    };

    struct Name_Space_Control {

        utils::Linked_List <Name_Space*>* name_spaces;

        int declarations_off;

        ~Name_Space_Control(); Name_Space_Control();

        bool addNameSpace(Name_Space*);
        
        bool addNameSpace(utils::Linked_List <char*>*);

        Name_Space* getNameSpace(utils::Linked_List <char*>*);

        Name_Space* getPreviousNameSpace(utils::Linked_List <char*>*);

        Name_Space* getNameSpaceOrAdd(utils::Linked_List <char*>*);       

    };

    struct Ast_Control {

        utils::Linked_List <Ast_Node_Name_Space*>* name_space_nodes;
        utils::Linked_List <char*>* implicit_values;

        utils::Linked_List <Ast_Node_Code_Block*>* code_block_chain;
        utils::Linked_List <Name_Space*>* name_space_chain;
        Name_Space_Control* name_space_control;
        int current_token_position;
        bool debug_info;      

        Tokenizer_Control* tokenizer_control;

        ~Ast_Control(); Ast_Control(Tokenizer_Control*, bool);

        Token* getToken(int); Ast_Node_Name_Space* getNameSpaceNodeFromNameSpace(Name_Space*);

        void printDebugInfo(const char*);

        void addNameSpace(Name_Space*); void popNameSpace();

        void addCodeBlockNode(Ast_Node_Code_Block*); void popCodeBlockNode();

        int addImplicitValue(char*);

        void generate();

    };

}

#endif