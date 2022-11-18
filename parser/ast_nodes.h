#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

/* This file holds the representation of any kinda of different action in code 
*  All representations are NODES 
*
*  Ast_Node -> It represents any type of node. Just hold a variable representing what type of node is
*
*  Ast_Node_Block_Code -> It represent any block of code. Usually representing any code inside {}, but is not mandatory
*   Holds more Nodes inside 
*   
*/

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct TypeInformation;
    struct Ast_Control;

    /* Node default representation for all types of nodes 
    *   Should never be declared by it self !!
    */
    struct Ast_Node { int id; /* Node type */ Ast_Node(int); };

    /**/
    #define AST_NODE_BLOCK_CODE 0 // Id
    #define AST_NODE_BLOCK_CODE_ENVIRONMENT_BLOCK_CODE 0
    struct Ast_Node_Block_Code : public Ast_Node {

        utils::LinkedList <Ast_Node*>* body; // All Nodes inside this block of code
        int environment; // Environment Type

        // Variables for Ast generation //
        utils::LinkedList <char*>* names;
        // End //
        // Function for Ast generation //
        int getNameDeclPos(char*);
        void addNewName(char*);
        // End //

        Ast_Node_Block_Code();

        static Ast_Node_Block_Code* generate(Ast_Control*, int); // Generate a Ast_Node_Block_Code

    };

    /* Represent a expression */
    #define AST_NODE_EXPRESSION 1 // ID
    struct Ast_Node_Expression : public Ast_Node {
        Ast_Node* frst, *scnd; // First and second values
        int id; // Token id of expression
        
        Ast_Node_Expression(Ast_Node*, Ast_Node*, int);

        static Ast_Node_Expression* generate(Ast_Control*); // Generate a Ast_Node_Expression

    };

    /* Represent a implicit value */
    #define AST_NODE_VALUE 2 // Id
    struct Ast_Node_Value : public Ast_Node {
        int valuePos, typeId; // Value position in storage || Token id type

        Ast_Node_Value(int, int);

        static Ast_Node_Value* generate(Ast_Control*); // Generate a Ast_Node_Value
    };

    /* Represent a variable declaration */
    #define AST_NODE_VARIABLE_DECLARATION 3
    struct Ast_Node_Variable_Declaration : public Ast_Node {
        int typePos, declId; // Position of type information || Name declaration id
        Ast_Node* value; // Value assigned to this variable | NULL if no value assigned
        Ast_Node_Variable_Declaration(int, int, Ast_Node*);

        /**/
        static Ast_Node_Variable_Declaration* getVariableDeclarationWithAssign(Ast_Control*, int, int, Ast_Node*);
        /**/
        static Ast_Node_Variable_Declaration* getVariableDeclarationWithOutAssign(Ast_Control*, int, int);

        static utils::LinkedList <Ast_Node*>* generate(Ast_Control*, TypeInformation*); // Generate a Ast_Node_Variable_Declaration
    };


}

#endif