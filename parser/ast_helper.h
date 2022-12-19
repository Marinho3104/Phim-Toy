#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Struct_Declaration;
    struct Ast_Node_Expression;
    struct Ast_Control;
    struct Ast_Node;

    struct Name_Space;

}

namespace parser_helper {

    struct Type_Information {

        parser::Ast_Node_Struct_Declaration* declaration;
        int pointer_level, reference_level, token_id;
        parser::Name_Space* name_space;

        ~Type_Information(); Type_Information(parser::Name_Space*, parser::Ast_Node_Struct_Declaration*, int, utils::Linked_List <int>*);

        bool operator==(Type_Information*); bool operator!=(Type_Information*);

        static Type_Information* generate();

        static Type_Information* generate(Type_Information*);

        int getByteSize();

    };

    struct Expression_Variable_Declaration {

        parser::Ast_Node_Variable_Declaration* declaration;
        Expression_Variable_Declaration* expression;
        int operator_id;

        ~Expression_Variable_Declaration(); Expression_Variable_Declaration(parser::Ast_Node_Variable_Declaration*, Expression_Variable_Declaration*, int);

        void replace(parser_helper::Type_Information*, int);

        static Expression_Variable_Declaration* generate(parser::Ast_Node_Expression*);

    };

    utils::Linked_List <parser::Ast_Node_Variable_Declaration*>* getVariableDeclarationOfLinkedList(utils::Linked_List <parser::Ast_Node*>*);

    utils::Linked_List <int>* getPointerOperations(bool);

    int getNodeType();

    utils::Linked_List <char*>* getScope();

    parser::Name_Space* getNameSpace(bool);

    int addName(char*);

    int getDeclarationId(char*, bool); bool isGlobalDeclaration(char*);

    parser::Ast_Node_Variable_Declaration* getVariableDeclaration(int, bool);

    parser::Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <parser::Ast_Node*>*, bool);
    
    parser::Ast_Node_Struct_Declaration* getStructDeclaration(int, bool);

    void addVariableDeclaration(parser::Ast_Node_Variable_Declaration*);

    void addFunctionDeclaration(parser::Ast_Node_Function_Declaration*);
    
    void addStructDeclaration(parser::Ast_Node_Struct_Declaration*);

    int getTokenTypeIdFromImplicitValueTokenType(int);

    int getPrimitiveTypeSize(int);

    parser::Ast_Node_Variable_Declaration* getVariableDeclarationTypeFromExpression(parser::Ast_Node_Expression*);

    int expressionPriority(int);    

}

#endif