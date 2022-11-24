
/* This file holds the representation of any kinda of different action in code (Node) */

#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Ast_Control;
    struct Type_Information;
    
    /* Node default representation for all types of nodes 
    *   Should never be declared by it self !!
    */
    struct Ast_Node { int node_id; /* Node type */ virtual ~Ast_Node(); Ast_Node(int); };

    /* Node Code Block represent any kinda of block of code */
    struct Ast_Node_Code_Block : public Ast_Node {

        utils::LinkedList <Ast_Node*>* body;
        int environment_id;

        utils::LinkedList <char*>* names_used;
        Ast_Node_Code_Block* previous_block;
        int names_count;

        /* Destructor */
        ~Ast_Node_Code_Block();

        /* Constructor */
        Ast_Node_Code_Block(int, utils::LinkedList <char*>*, Ast_Node_Code_Block*, int);

        /* Generator */
        static Ast_Node_Code_Block* generate(Ast_Control*, int, utils::LinkedList <char*>*);

        /* Generate body */
        void generateBody(Ast_Control*);

        /* Get new node */
        static utils::LinkedList <parser::Ast_Node*>* getNewNodes(Ast_Control*);

        /* Return declaration id (a.k.a position in Linked List + names_count) 
        *   @param __n Name to search for declaration id
        *   @param __glb Variable is set to true if declaration id is in global scope
        */
        int getDeclarationId(char*, bool&);

        /* Add new name into linked list 
        *   Returns true if name was succesfully added to linked list 
        *   or false if not (name already were in linked list)
        *   @param __n Name to add 
        *   @param __cpy Makes a new copy of given input
        */
        bool addNewName(char*, bool);
    };

    /* Node Expression represents any kinda of expression */
    struct Ast_Node_Expression : public Ast_Node {

        Ast_Node* first;
        Ast_Node_Expression* second;
        int expression_id;

        /* Destructor */
        ~Ast_Node_Expression();

        Ast_Node_Expression(Ast_Node*, Ast_Node_Expression*, int);

        static Ast_Node_Expression* generate(Ast_Control*);

        int getByteSize();

        static Ast_Node* getFirstNode(Ast_Control*);

        static Ast_Node_Expression* getSecondNode(Ast_Control*);

    };

    /* Node Value representa implicit value */
    struct Ast_Node_Value : public Ast_Node {

        int value_position, token_id; // Value position in storage || Token id type

        ~Ast_Node_Value() = default;

        /* Constructor */ 
        Ast_Node_Value(int, int);

        /* Generator */
        static Ast_Node_Value* generate(Ast_Control*);
    
        /* Return size of value */
        int getByteSize();

        /* Return type id */
        Type_Information* getType();

    };

    /* Node Variable Declaration represents a variable declaration to Stack memory */
    struct Ast_Node_Variable_Declaration : public Ast_Node {

        Type_Information* variable_type; // variable type information 
        int declaration_id; //  Name declaration id

        /* Destructor */
        ~Ast_Node_Variable_Declaration();
        /* Constructor */
        Ast_Node_Variable_Declaration(Type_Information*, int);

        /* Generator */
        static utils::LinkedList <Ast_Node*>* generate(Ast_Control*, Type_Information*);

        /* Return size of variable type */
        int getByteSize();

        /* Return size of variable type */
        Type_Information* getType(utils::LinkedList <Type_Information*>*);

    };

    /* Node Variable represent the use of value of a variable name */
    struct Ast_Node_Variable : public Ast_Node {
        int declaration_id; // Declaration id
        bool is_global_variable; // is global variable
        Ast_Node_Variable_Declaration* variable_declaration; // Variable declaration info
        /* Destructor */
        ~Ast_Node_Variable() = default;
        /* Constructor */
        Ast_Node_Variable(int, bool);

        /* Generator */
        static Ast_Node_Variable* generate(Ast_Control*);

        /* Return Size of type */
        int getByteSize();

    };

    /* Represent any pointer operation at node */
    struct Ast_Node_Pointer_Operators : public Ast_Node {

        Ast_Node* value;
        int pntrLvl;
        /* Destructor */
        ~Ast_Node_Pointer_Operators();
        /* Constructor */
        Ast_Node_Pointer_Operators(utils::LinkedList <int>*, Ast_Node*);
        /* Generator */
        static Ast_Node_Pointer_Operators* generate(Ast_Control*, utils::LinkedList <int>*);

        // Ast Generation Only //

        static Ast_Node* getValue(Ast_Control*);

        // Ast Generation Only //

        /**/
        int getTypeSize(); 

    };

}

#endif