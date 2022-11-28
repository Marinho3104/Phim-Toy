
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
        int getDeclarationId(char*);

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
        Type_Information* getType();

    };

    /* Node Variable represent the use of value of a variable name */
    struct Ast_Node_Variable : public Ast_Node {
        int declaration_id; // Declaration id
        Ast_Node_Variable_Declaration* variable_declaration; // Variable declaration info
        /* Destructor */
        ~Ast_Node_Variable() = default;
        /* Constructor */
        Ast_Node_Variable(int);

        /* Generator */
        static Ast_Node_Variable* generate(Ast_Control*);

        /* Return Size of type */
        int getByteSize();

        /* Return type */
        Type_Information* getType();

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
        int getByteSize(); 

    };

    /* Represent a assign to a variable */
    struct Ast_Node_Variable_Assignment : public Ast_Node {
        bool operation_is_left; // Is operator in left side of variable 
        int expression_id; // Expression Id
        Ast_Node* value_before_assign; // Value Before Assign
        Ast_Node_Expression *value; // Value assign to || NULL if is single assignment

        /* destructor */
        ~Ast_Node_Variable_Assignment();
        /* constructor */
        Ast_Node_Variable_Assignment(bool, Ast_Node*, int, Ast_Node_Expression*);

        /* Generator */
        static Ast_Node_Variable_Assignment* generate(Ast_Control*);

        static Ast_Node* getValueBeforeAssign(Ast_Control*);
        
        static Ast_Node_Expression* getValue(Ast_Control*);

    };

    /* Represent any parenthesis */
    struct Ast_Node_Parenthesis : public Ast_Node {
        Ast_Node_Expression* value; // Value inside parenthesis
        
        /* Destructor */
        ~Ast_Node_Parenthesis();
        /* Constructor */
        Ast_Node_Parenthesis(Ast_Node_Expression*);

        /* Generator */
        static Ast_Node_Parenthesis* generate(Ast_Control*);

        int getByteSize();

        Type_Information* getType();

    };

    /* Represent a function declaration */
    struct Ast_Node_Function_Declaration : public Ast_Node {
        
        Type_Information* return_type; // Return type
        int declaration_id, body_pos; // Declaration id 
        utils::LinkedList <Ast_Node*>* parameters; // Parameters of function
        Ast_Node_Code_Block* body; // Function body || NULL If is forward declaration

        /* Destructor */
        ~Ast_Node_Function_Declaration();         
        /* Constructor */
        Ast_Node_Function_Declaration(Type_Information*, int, utils::LinkedList <Ast_Node*>*, Ast_Node_Code_Block*);
        /* Generator */
        static Ast_Node_Function_Declaration* generate(Ast_Control*, Type_Information*);
        
        static utils::LinkedList <Ast_Node*>* getParameters(Ast_Control*, utils::LinkedList <char*>*);

        int getByteSize();
        
    };

    /* Represent a function call */
    struct Ast_Node_Function_Call : public Ast_Node {
        Ast_Node_Function_Declaration* function_declaration; // Function declaration
        utils::LinkedList <Ast_Node_Expression*>* parameters; // Parameters of function
        int declaration_id; // Id
        /* Destructor */
        ~Ast_Node_Function_Call();
        /* Constructor */
        Ast_Node_Function_Call(int, utils::LinkedList <Ast_Node_Expression*>*);
        
        /* Generator */
        static Ast_Node_Function_Call* generate(Ast_Control*);

        static utils::LinkedList <Ast_Node_Expression*>* getFunctionCallParameters(Ast_Control*);

        int getByteSize();

    };

    /* Represent a struct declaration */
    struct Ast_Node_Struct_Declaration : public Ast_Node {
        int declaration_id; // Struct Declaration id
        bool is_contract; // Is contract
        Ast_Node_Code_Block* body_info; // just to hold info of body
        utils::LinkedList <Ast_Node*>* fields;
        utils::LinkedList <Ast_Node_Function_Declaration*>* functions;

        ~Ast_Node_Struct_Declaration();

        Ast_Node_Struct_Declaration(int, bool);
        
        /* Generator */
        static Ast_Node_Struct_Declaration* generate(Ast_Control*);

        void ignoreCodeBlock(Ast_Control*);

        void getFields(Ast_Control*);

        void getFunctions(Ast_Control*); 

        int getByteSize();

    };


}

#endif