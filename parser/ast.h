#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

    // Forward
    struct Name_Space_Control;
    struct Ast_Control;

    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Struct_Declaration;
    struct Ast_Node_Code_Block;
    struct Ast_Node_Name_Space;
    struct Ast_Node;

    struct Tokenizer_Control;
    struct Token;


    extern Name_Space_Control* name_space_control;
    extern Tokenizer_Control* tokenizer_control;
    extern Ast_Control* built_in_ast_control;
    extern Ast_Control* ast_control;


    struct Declaration_Tracker {

        utils::Linked_List <Ast_Node_Variable_Declaration*>* variable_declarations;
        utils::Linked_List <Ast_Node_Function_Declaration*>* function_declarations;
        utils::Linked_List <Ast_Node_Struct_Declaration*>* struct_declarations;

        utils::Linked_List <int>* declaration_ids;
        utils::Linked_List <char*>* names;

        int* off;

        ~Declaration_Tracker(); Declaration_Tracker();

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);

        Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);

        Ast_Node_Struct_Declaration* getStructDeclaration(int);

        void addName(char*);

        int getDeclarationId(char*);

    };

    struct Name_Space {

        Declaration_Tracker* declaration_tracker;
        utils::Linked_List <char*>* scope;

        ~Name_Space(); Name_Space(utils::Linked_List <char*>*);

    };

    struct Name_Space_Control {

        utils::Linked_List <Name_Space*>* name_spaces;
        int declarations_off;

        ~Name_Space_Control(); Name_Space_Control();

        void addNameSpace(Name_Space*);

        void addNameSpace(utils::Linked_List <char*>*);

        Name_Space* getNameSpace(utils::Linked_List <char*>*);

        Name_Space* getNameSpaceOrAdd(utils::Linked_List <char*>*);

        Name_Space* getPreviousNameSpace(Name_Space*);

    };

    struct Ast_Control {

        utils::Linked_List <Ast_Node_Name_Space*>* name_space_nodes;
        utils::Linked_List <char*>* implicit_values;

        utils::Linked_List <Ast_Node_Name_Space*>* name_space_chain;
        utils::Linked_List <Ast_Node_Code_Block*>* code_block_chain;

        int current_token_position;
        bool debug_mode;


        ~Ast_Control(); Ast_Control(bool);

        Token* getToken(int); void printDebugInfo(const char*);

        void addNameSpaceNode(Ast_Node_Name_Space*);

        Ast_Node_Name_Space* getNameSpaceNodeFromNameSpace(Name_Space*);

        Ast_Node_Name_Space* getNameSpaceNodeFromNameSpaceScope(utils::Linked_List <char*>*);

        void addNameSpaceToChain(Name_Space*); void addNameSpaceToChain(Ast_Node_Name_Space*); void popNameSpaceFromChain();

        void addCodeBlockToChain(Ast_Node_Code_Block*); void popCodeBlockFromChain();

        int addImplicitValue(char*);

        void generate();

    };

    void setUpAst(bool);

    void cleanAst();

}

#endif