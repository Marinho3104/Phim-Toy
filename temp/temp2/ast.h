#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

    // Forward Ast's
    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Struct_Declaration;
    struct Ast_Node_Name_Space;
    struct Ast_Node_Code_Block;
    struct Ast_Node;

    // Forward other
    struct Tokenizer_Control;
    struct Token;


    struct Declaration_Tracker {

        utils::Linked_List <Ast_Node_Variable_Declaration*>* variable_declarations;
        utils::Linked_List <Ast_Node_Function_Declaration*>* function_declarations;
        utils::Linked_List <Ast_Node_Struct_Declaration*>* struct_declarations;

        utils::Linked_List <int>* declaration_ids;
        utils::Linked_List <char*>* names;

        int* off;

        ~Declaration_Tracker(); Declaration_Tracker();

        void updateOff(int*);

        void addDeclaration(Ast_Node_Variable_Declaration*); Ast_Node_Variable_Declaration* getVariableDeclaration(int);
        
        void addDeclaration(Ast_Node_Function_Declaration*); Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);
        
        void addDeclaration(Ast_Node_Struct_Declaration*); Ast_Node_Struct_Declaration* getStructDeclaration(int);

        int addName(char*);

        int getDeclarationId(char*);

    };

    struct Name_Space {

        Declaration_Tracker* declaration_tracker;
        utils::Linked_List <char*>* scope;

        ~Name_Space(); Name_Space(utils::Linked_List <char*>*);

        void printScope();

        void updateOff(int*);

    };

    struct Name_Space_Control {

        utils::Linked_List <Name_Space*>* name_spaces;

        int declaration_off;

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

        utils::Linked_List <Ast_Node_Name_Space*>* name_space_node_chain;
        utils::Linked_List <Ast_Node_Code_Block*>* code_block_node_chain;

        Tokenizer_Control* tokenizer_control;

        int current_token_position;
        bool debug_info;

        ~Ast_Control(); Ast_Control(Tokenizer_Control*, bool);

        Token* getToken(int);

        void printDebugInfo(const char*);

        void addNameSpaceNodeToChain(Ast_Node_Name_Space*); void popNameSpaceNodeOfChain();

        int addImplicitValue(char*);

        void generate();

    };

    extern Name_Space_Control* name_space_control;
    extern Ast_Control* ast_control;

    void setUpAst(Tokenizer_Control*, bool);
    
    void cleanAst();

}

#endif
