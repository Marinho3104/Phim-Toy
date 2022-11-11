#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Ast_Node_Block;
    struct Ast_Control;
    struct Ast_Node;
    struct Storage;
    struct Token;

    /**/
    struct TypeInformation {

        bool userDefinedType;
        int typePos, pntrLevel, rfrnLevel;

        ~TypeInformation() = default;

        TypeInformation(bool, int, int, int);

        bool operator==(TypeInformation&);

        static TypeInformation* generate(Ast_Control*, Ast_Node_Block*);

    };

    #define ENVIRONMENT_TYPE_GLOBAL 0
    #define ENVIRONMENT_TYPE_FUNCTION 1
    #define ENVIRONMENT_TYPE_STRUCT 2
    #define ENVIRONMENT_TYPE_BLOCKOFCODE 3
    /* Enviroment is need to keep track of every modification */
    struct Environment {

        Storage* storage; // Hold every information in environment
        unsigned char type; // Environment type
        utils::LinkedList <int>* variablesNamesDeclared, *functionsNamesDeclared, *structsNamesDeclared;
        Environment(unsigned char);
        /* Check if name pos is declared in Environment */
        bool checkIfNameIsDeclared(int);
        /* Check if name already declared in environment if not added to variablesNamesDeclared field
        *   @return True if name was added to variablesNamesDeclared field
        */
        bool addVaribleNameDeclared(int);
        /**/
        bool checkVariableNameIsDeclared(int);
        /* Check if name already declared in environment if not added to functionsNamesDeclared field
        *   @return True if name was added to functionsNamesDeclared field
        */
        bool addFunctionNameDeclared(int);
        /**/
        bool checkFunctionNameIsDeclared(int);
        /* Check if name already declared in environment if not added to structsNamesDeclared field
        *   @return True if name was added to structsNamesDeclared field
        */
        bool addStructNameDeclared(int);
        /**/
        bool checkStructNameIsDeclared(int);
    };

    /* Store every information to avoid duplicade information */
    struct Storage {

        utils::LinkedList <char*>* names, *values; // Names useds cross Tokens || Values useds cross tokens
        utils::LinkedList <int>* expressionsId, *keyWordsId; // Token id of expressions || Token id of keyword
        utils::LinkedList <TypeInformation*>* types; // Types useds cross Tokens 

        Storage();
        /* Add new name into Linked List
        *   If name already exists dont add
        *   @return Pos of name given in Linked List 
        */
        int addNewName(char*);
        /* Add new value into Linked List
        *   If value already exists dont add
        *   @return Pos of value given in Linked List 
        */
        int addNewValue(char*);
        /* Add new expression id into Linked List
        *   If expression id already exists dont add
        *   @return Pos of expression id given in Linked List 
        */
        int addNewExpressionId(int);
        /* Add new keyword id into Linked List
        *   If expression id already exists dont add
        *   @return Pos of keyword id given in Linked List 
        */
        int addNewKeyWordId(int);
        /**/
        int addNewType(TypeInformation*);

    };
    /* Ast Control Exception */
    struct Ast_Control_Exception {
        char* description; // Exception description
        Ast_Control_Exception(char*);
    };
    /* Ast Control */
    struct Ast_Control {

        utils::LinkedList <parser::Token*>* tokensColl;
        Ast_Node_Block* globalBlock; // Its the hole code
        Storage* globalStorage; // Store all information used accross the code
        int* crrntPos;

        Ast_Control(utils::LinkedList <parser::Token*>*);
        /**/
        utils::LinkedList <Ast_Node*>* getNewNodes(Ast_Node_Block*, bool);
        /**/
        void generateAst();
        
    };
}

#endif