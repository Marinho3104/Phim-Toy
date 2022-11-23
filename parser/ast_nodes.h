
/* This file holds the representation of any kinda of different action in code (Node) */

#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Type_Information; // Type Infor,ation
    struct Ast_Control; // Ast Control
    struct Ast_Node_Variable_Declaration; // Ast Variable Declaration
    struct Ast_Node_Function_Declaration; // Ast Function Declaration

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
        Ast_Node_Code_Block* previousBlock;
        int namesCount;
        
        Ast_Node_Code_Block(int, utils::LinkedList <char*>*, Ast_Node_Code_Block*, int);

        static utils::LinkedList <parser::Ast_Node*>* getNewNodes(Ast_Control*);

        int getDeclarationId(char*);

        int getDeclarationIdCurrntBlock(char*);

        /* Add new name into linked list 
        *   Returns true if name was succesfully added to linked list 
        *   or false if not (name already were in linked list)
        */
        bool addNewName(char*);
        
        // Ast Generation Only //

        /* Constructor */
        Ast_Node_Code_Block(utils::LinkedList <Ast_Node*>*, int);

        /* Generator */
        static Ast_Node_Code_Block* generate(Ast_Control*, int, utils::LinkedList <char*>*);

    };

    /* Node Expression represents any kinda of expression */
    struct Ast_Node_Expression : public Ast_Node {

        Ast_Node* frst, *scnd; // First and Second values
        int expId; // Expression operator id 

        /* Constructor */
        Ast_Node_Expression(Ast_Node*, Ast_Node*, int);

        /* Return size result of expression */
        int getResultSize(utils::LinkedList <Ast_Node_Variable_Declaration*>*, utils::LinkedList <Ast_Node_Function_Declaration*>*, utils::LinkedList <Type_Information*>*);

        // Ast Generation Only //

        static Ast_Node* getFirstNode(Ast_Control*);
        static Ast_Node* getSecondNode(Ast_Control*);

        // Ast Generation Only //

        /* Generator */
        static Ast_Node_Expression* generate(Ast_Control*);

    }; 

    /* Node Value representa implicit value */
    struct Ast_Node_Value : public Ast_Node {
        int valuePos, tkId; // Value position in storage || Token id type

        /* Constructor */ 
        Ast_Node_Value(int, int);

        /* Return size of value */
        int getTypeSize();

        /* Return type id */
        Type_Information* getType();

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

        /* Return size of variable type */
        int getTypeSize(utils::LinkedList <Type_Information*>*);

        /* Return size of variable type */
        Type_Information* getType(utils::LinkedList <Type_Information*>*);

        /* Generator */
        static utils::LinkedList <Ast_Node*>* generate(Ast_Control*, Type_Information*);

    };

    /* Node Variable represent the use of value of a variable name */
    struct Ast_Node_Variable : public Ast_Node {
        int declId; // Declaration position
        Ast_Node_Variable(int);

        // Ast Generation Only //
        // Ast Generation Only //

        /* Return Size of type */
        int getTypeSize(utils::LinkedList <Ast_Node_Variable_Declaration*>*, utils::LinkedList <Type_Information*>*);

        /**/
        static Ast_Node_Variable* generate(Ast_Control*);
    };

    /* Represent any pointer operation at node */
    struct Ast_Node_Pointer_Operators : public Ast_Node {

        utils::LinkedList <int>* operations; // All operators before variable -> * &
        Ast_Node* value;
        int pntrLvl;
        Ast_Node_Pointer_Operators(utils::LinkedList <int>*, Ast_Node*);

        // Ast Generation Only //

        static Ast_Node* getNewNode(Ast_Control*);

        // Ast Generation Only //

        /**/
        int getTypeSize(); 

        static Ast_Node_Pointer_Operators* generate(Ast_Control*, utils::LinkedList <int>*);

    };

    /* Represent a assign to a variable */
    struct Ast_Node_Variable_Assignment : public Ast_Node {
        bool opIsLeft; // Is operator in left side of variable 
        int expId; // Expression Id
        Ast_Node* valueBeforeAssign, *value; // Value Before Assign | Value assign to || NULL if is single assignment

        Ast_Node_Variable_Assignment(bool, Ast_Node*, int, Ast_Node*);

        // Ast Generation Only //

        static Ast_Node* getValueBeforeAssign(Ast_Control*);
        
        static Ast_Node* getValue(Ast_Control*);

        // Ast Generation Only //

        /**/
        static Ast_Node_Variable_Assignment* generate(Ast_Control*);

    };

    /* Represent any parenthesis */
    struct Ast_Node_Parenthesis : public Ast_Node {
        Ast_Node* value; // Value inside parenthesis
        Ast_Node_Parenthesis(Ast_Node*);

        // Ast Generation Only //
        // Ast Generation Only //

        int getTypeSize(utils::LinkedList <Ast_Node_Variable_Declaration*>*, utils::LinkedList <Ast_Node_Function_Declaration*>*, utils::LinkedList <Type_Information*>*);

        /* Generator */
        static Ast_Node_Parenthesis* generate(Ast_Control*);
    };

    /* Represent a function declaration */
    struct Ast_Node_Function_Declaration : public Ast_Node {
        int typeRtrPos, declId; // Type return position || Name position
        utils::LinkedList <Ast_Node*>* parameters; // Parameters of function
        Ast_Node* body; // Function body || NULL If is forward declaration
        /* Constructor */
        Ast_Node_Function_Declaration(int, int, utils::LinkedList <Ast_Node*>*, Ast_Node*);
        
        // Ast Generation Only //

        static utils::LinkedList <Ast_Node*>* getParameters(Ast_Control*, utils::LinkedList <char*>*);

        // Ast Generation Only //

        int getTypeSize(utils::LinkedList <Type_Information*>*);
        
        /* Generator */
        static Ast_Node_Function_Declaration* generate(Ast_Control*, Type_Information*);
    };

    /* Represent a function call */
    struct Ast_Node_Function_Call : public Ast_Node {
        int declId; // Id
        utils::LinkedList <Ast_Node*>* parameters; // Parameters of function
        /* Constructor */
        Ast_Node_Function_Call(int, utils::LinkedList <Ast_Node*>*);

        // Ast Generation Only //
        static utils::LinkedList <Ast_Node*>* getFunctionCallParameters(Ast_Control*);
        // Ast Generation Only //

        int getTypeSize(utils::LinkedList <Ast_Node_Function_Declaration*>*, utils::LinkedList <Type_Information*>*);
        
        /* Generator */
        static Ast_Node_Function_Call* generate(Ast_Control*);

    };

    /* Represent a struct declaration */
    struct Ast_Node_Struct_Declaration : public Ast_Node {
        int declId; // Struct Declaration id
        bool isContract; // Is contract
        Ast_Node* body; // Body || NULL if is forward declaration

        Ast_Node_Struct_Declaration(int, bool, Ast_Node*);
        
        /* Generator */
        static Ast_Node_Struct_Declaration* generate(Ast_Control*);
    };

    /* Represent end of code */
    struct Ast_Node_End : public Ast_Node {
        
        Ast_Node_End();

    };

}



#endif