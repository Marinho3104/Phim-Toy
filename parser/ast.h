#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Ast_Node_Code_Block; // Code Block Node
    struct Ast_Control; // Ast Control
    struct Ast_Node; // Default Node
    struct Token; // Token 

    struct Type_Information {
        utils::LinkedList <int>* pntrOprns; // Pointer Operations type
        int id, usrDefDeclId, pntrLvl, rfrnLvl; // Identification of Type, if is identifier means is user defined | If is user defined this indicates the Declaration Id

        /* Destructor */
        ~Type_Information() = default;
        /* Constructor */
        Type_Information(int, int, utils::LinkedList <int>*);

        bool operator==(Type_Information&);

        /* Return byte size of type */
        int getByteSize();

        /* Generator */
        static Type_Information* generate(Ast_Control*);

    };

    /* Storage */
    struct Storage {
        utils::LinkedList <Type_Information*>* types; // Types used
        utils::LinkedList <char*>* implicitValues; // Implicit values used 

        /* Destructor */
        ~Storage() = default; // TODO change des
        /* Constructor */
        Storage();
        /* Add new value into Linked List
        *   If value already exists dont add
        *   @return Pos of value given in Linked List 
        */
        int addNewValue(char*);
        /* Add new type into Linked List
        *   If type already exists dont add
        *   @return Pos of type given in Linked List 
        */
        int addNewType(Type_Information*);

    };

    struct Ast_Control {

        utils::LinkedList <parser::Ast_Node*>* code_blocks; // All blocks of code
        utils::LinkedList <parser::Token*>* tokensColl; // Tokens Collection
        parser::Ast_Node_Code_Block* crrntBlock; // Current block
        Storage* storage; // Storage
        int crrntTkPos; // Keeps track of current token | Names count

        /* Constructor */
        Ast_Control(utils::LinkedList <parser::Token*>*);

        /* Generate Ast Nodes from the given tokens collection */
        void generateAst();

    };

}

#endif