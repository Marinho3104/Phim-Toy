
/* This file holds the representation of any kinda of different action in code (Node) */

#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Type_Information; // Type Infor,ation
    struct Ast_Control; // Ast Control

    /* Node default representation for all types of nodes 
    *   Should never be declared by it self !!
    */
    struct Ast_Node { int id; /* Node type */ Ast_Node(int); };

    /* Node Code Block represent any kinda of block of code */
    struct Ast_Node_Code_Block : public Ast_Node {

        utils::LinkedList <Ast_Node*>* body; // All code inside block code
        /* Id of environment of code block
        *   0 -> Normal
        *   MORE ... TODO
        */
        int environment;

        // Ast Generation Only //

        utils::LinkedList <char*>* namesUsedInBlock;
        
        Ast_Node_Code_Block(int);

        static utils::LinkedList <parser::Ast_Node*>* getNewNodes(Ast_Control*);

        int getDeclarationId(char*);

        /* Add new name into linked list 
        *   Returns true if name was succesfully added to linked list 
        *   or false if not (name already were in linked list)
        */
        bool addNewName(char*);
        
        // Ast Generation Only //

        /* Constructor */
        Ast_Node_Code_Block(utils::LinkedList <Ast_Node*>*, int);

        /* Generator */
        static Ast_Node_Code_Block* generate(Ast_Control*, int);

    };

    /* Node Expression represents any kinda of expression */
    struct Ast_Node_Expression : public Ast_Node {

        Ast_Node* frst, *scnd; // First and Second values
        int expId; // Expression operator id 

        /* Constructor */
        Ast_Node_Expression(Ast_Node*, Ast_Node*, int);

        // Ast Generation Only //

        static Ast_Node* getFirstNode(Ast_Control*);
        static Ast_Node* getSecondNode(Ast_Control*);

        // Ast Generation Only //

        /* Generator */
        static Ast_Node_Expression* generate(Ast_Control*);

    }; 

    /* Node Value representa implicit value */
    struct Ast_Node_Value : public Ast_Node {
        int valuePos, id; // Value position in storage || Token id type

        /* Constructor */ 
        Ast_Node_Value(int, int);

        /* Generator */
        static Ast_Node_Value* generate(Ast_Control*);
    
    };

    /* Node Variable Declaration represents a variable declaration to Stack memory */
    struct Ast_Node_Variable_Declaration : public Ast_Node {

        int typePos, declId; // Position of type information || Name declaration id

        /* Destructor */
        ~Ast_Node_Variable_Declaration() = default; // TODO
        /* Constructor */
        Ast_Node_Variable_Declaration(int, int);

        // Ast Generation Only //
        // Ast Generation Only //

        /* Generator */
        static utils::LinkedList <Ast_Node*>* generate(Ast_Control*, Type_Information*);

    };

    /* Node Variable represent the use of value of a variable name */
    struct Ast_Node_Variable : public Ast_Node {
        int declId; // Declaration position
        Ast_Node_Variable(int);

        // Ast Generation Only //
        // Ast Generation Only //

        /**/
        static Ast_Node_Variable* generate(Ast_Control*);
    };

    /* Represent any pointer operation at node */
    struct Ast_Node_Pointer_Operators : public Ast_Node {

        utils::LinkedList <int>* operations; // All operators before variable -> * &
        Ast_Node* value;
        Ast_Node_Pointer_Operators(utils::LinkedList <int>*, Ast_Node*);

        // Ast Generation Only //

        static Ast_Node* getNewNode(Ast_Control*);

        // Ast Generation Only //


        static Ast_Node_Pointer_Operators* generate(Ast_Control*, utils::LinkedList <int>*);

    };

    /* Represent a assign to a variable */
    struct Ast_Node_Variable_Assignment : public Ast_Node {
        bool opIsLeft; // Is operator in left side of variable 
        int id; // Expression Id
        Ast_Node* valueBeforeAssign, *value; // Value Before Assign | Value assign to || NULL if is single assignment

        Ast_Node_Variable_Assignment(bool, Ast_Node*, int, Ast_Node*);

        // Ast Generation Only //

        static Ast_Node* getValueBeforeAssign(Ast_Control*);
        
        static Ast_Node* getValue(Ast_Control*);

        // Ast Generation Only //

        /**/
        static Ast_Node_Variable_Assignment* generate(Ast_Control*);

    };

}

#endif