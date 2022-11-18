#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Ast_Node_Block_Code;
    struct Ast_Control;
    struct Ast_Node;
    struct Token;


    /* Type Information */
    struct TypeInformation {
        bool userDefined;
        int typeId, pntrLvl, rfrnLvl;
        ~TypeInformation() = default;
        TypeInformation(bool, int, int, int);

        bool operator==(TypeInformation&);

        static TypeInformation* generate(Ast_Control*);
    };

    /* Storage */
    struct Storage {

        utils::LinkedList <TypeInformation*>* types; // Types used in code
        utils::LinkedList <char*>* values; // Values used in code

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
        int addNewType(TypeInformation*);

    };

    /* Ast Control */
    struct Ast_Control {

        utils::LinkedList <parser::Ast_Node*>* blockCodes; // Hold all block of codes | last block is global block
        utils::LinkedList <parser::Token*>* tokensColl; // Tokens
        parser::Storage* storage; // Storage 
        parser::Ast_Node_Block_Code* crrntBlock; // Current Block of code
        int crrntTk;

        Ast_Control(utils::LinkedList <parser::Token*>*);
        /* Return nodes from current tokens */
        utils::LinkedList <Ast_Node*>* getNewNodes(bool);
        /* Generate Ast Nodes of given tokens */
        void generateAst();
        
    };

}

#endif