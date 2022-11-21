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

    /* Represent a variable value */
    #define AST_NODE_VARIABLE 4
    struct Ast_Node_Variable : public Ast_Node {
        
        int declId; // Declaration position
        Ast_Node_Variable(int);
        /**/
        static Ast_Node_Variable* generate(Ast_Control*);
    };

    /* Represent any pointer operation at node */
    #define AST_NODE_POINTER_OPERATORS 5
    struct Ast_Node_Pointer_Operators : public Ast_Node {

        utils::LinkedList <int>* operations; // All operators before variable -> * &
        Ast_Node* value;
        Ast_Node_Pointer_Operators(utils::LinkedList <int>*, Ast_Node*);

        static Ast_Node_Pointer_Operators* generate(Ast_Control*);

    };

    /* Represent a assign to a variable */
    #define AST_NODE_VARIABLE_ASSIGNMENT 6
    struct Ast_Node_Variable_Assignment : public Ast_Node {
        bool opIsLeft; // Is operator in left side of variable 
        int id; // Expression Id position
        Ast_Node* valueBeforeAssign, *value; // Value Before Assign | Value assign to || NULL if is single assignment

        Ast_Node_Variable_Assignment(bool, Ast_Node*, int, Ast_Node*);
        /**/
        static Ast_Node* generateValueBeforeAssign(Ast_Control*);
        /**/
        static Ast_Node_Variable_Assignment* generate(Ast_Control*);

    };

}



#endif