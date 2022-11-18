#include "./ast_nodes.h"

#include "./../utils/linkedList.h" // Linked list
#include "./ast_helper.h" // Ast Helper
#include "./token.h" // Token Info
#include "./ast.h" // Ast control 
 
#include <iostream>

/*      Ast Node     */

parser::Ast_Node::Ast_Node(int _id) : id(_id) {}

/*      Ast Node Block Code     */

parser::Ast_Node_Block_Code::Ast_Node_Block_Code() : Ast_Node(AST_NODE_BLOCK_CODE) {
    body = new utils::LinkedList <Ast_Node*>();
    names = new utils::LinkedList <char*>();
}

int parser::Ast_Node_Block_Code::getNameDeclPos(char* _) { return names->getObjectPosition(_, NULL); }

void parser::Ast_Node_Block_Code::addNewName(char* _) { if (names->getObjectPosition(_, NULL) == -1) names->add(_); }

parser::Ast_Node_Block_Code* parser::Ast_Node_Block_Code::generate(parser::Ast_Control* _astCntrl, int _environmentType) {
    std::cout << "--> Ast Node Block Code <--" << std::endl;
    parser::Ast_Node_Block_Code* _ = (parser::Ast_Node_Block_Code*) malloc(sizeof(parser::Ast_Node_Block_Code));
    new(_) parser::Ast_Node_Block_Code();
    _astCntrl->crrntBlock = _;
    int _lstS = 0;

    while(_astCntrl->crrntTk < _astCntrl->tokensColl->count) {

        _->body->join(
            _astCntrl->getNewNodes(
                0
            )
        );

        if (_->body->count == _lstS) break;

        _lstS = _->body->count;

    }
    std::cout << "--> End of Ast Node Block Code <--" << std::endl;

    return _;

}

/*      Ast Node Expression     */

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node* _f, Ast_Node* _s, int _id) 
    : Ast_Node(AST_NODE_EXPRESSION), frst(_f), scnd(_s), id(_id) {}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(Ast_Control* _astCntrl) {
    std::cout << "--> Ast Node Expression <--" << std::endl;

    parser::Ast_Node_Expression* _ = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    utils::LinkedList<Ast_Node*>* _frstValue, *_scndValue;
    int _expressionId;

    _frstValue = _astCntrl->getNewNodes(1);

    _expressionId = (*_astCntrl->tokensColl)[_astCntrl->crrntTk]->id;

    switch (_expressionId)
    {
        case TOKEN_ENDINSTRUCTION:
        case TOKEN_CLOSEPARENTHESES:
        case TOKEN_COMMA:
            _expressionId = -1;
            _scndValue = (utils::LinkedList <Ast_Node*>*) malloc(sizeof(utils::LinkedList <Ast_Node*>)); 
            new (_scndValue) utils::LinkedList <Ast_Node*>();
            break;
        default: 
            _astCntrl->crrntTk++;
            _scndValue = _astCntrl->getNewNodes(0);
            break;
    } 

    new (_) parser::Ast_Node_Expression(
        !_frstValue->count ? NULL : _frstValue->frst->object,
        !_scndValue->count ? NULL : _scndValue->frst->object,
        _expressionId
    );

    // Leak of memory in DataLL needs to call function

    free(_frstValue);
    free(_scndValue);

    return _;

}

/*      Ast Node Value     */

parser::Ast_Node_Value::Ast_Node_Value(int _valuePos, int _typeId) : Ast_Node(AST_NODE_VALUE), valuePos(_valuePos), typeId(_typeId) {}

parser::Ast_Node_Value* parser::Ast_Node_Value::generate(parser::Ast_Control* _astCntrl) {
    
    std::cout << "--> Ast Node Value <--" << std::endl;
    parser::Ast_Node_Value* _ = (parser::Ast_Node_Value*) malloc(sizeof(parser::Ast_Node_Value));

    new(_) parser::Ast_Node_Value(
        _astCntrl->storage->addNewValue(
            (*_astCntrl->tokensColl)[_astCntrl->crrntTk]->phr
        ), (*_astCntrl->tokensColl)[(_astCntrl->crrntTk)++]->id
    );

    return _;

}

/*      Ast Node Variable Declaration     */

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(int _typePos, int _declId, Ast_Node* _value) 
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION), typePos(typePos), declId(_declId), value(_value) {}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithAssign(
    Ast_Control* _astCntrl, int _declId, int _typePos, Ast_Node* _value) {

            std::cout << "Added with assigment" << std::endl;
            parser::Ast_Node_Variable_Declaration* _ = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));

            new (_) parser::Ast_Node_Variable_Declaration(
                _typePos, _declId, _value
            );

            return _;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithOutAssign(
    Ast_Control* _astCntrl, int _declId, int _typePos) {

        std::cout << "Added, no assigment" << std::endl;

        parser::Ast_Node_Variable_Declaration* _ = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));

        new (_) parser::Ast_Node_Variable_Declaration(
            _typePos, _declId, NULL
        );

        return _;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate(Ast_Control* _astCntrl, TypeInformation* _type) {
    std::cout << "--> Ast Node Variable Declaration <--" << std::endl;
    utils::LinkedList <parser::Ast_Node*>* _rtr = new utils::LinkedList <parser::Ast_Node*>();
    int _pntrLevel, _rfrnLevel, _typeInfoPos, _declId;
    while((*_astCntrl->tokensColl)[(_astCntrl->crrntTk)]->id != TOKEN_ENDINSTRUCTION) {

        _typeInfoPos = _astCntrl->storage->addNewType(_type);
        if ((_declId = _astCntrl->crrntBlock->getNameDeclPos((*_astCntrl->tokensColl)[_astCntrl->crrntTk]->phr)) != -1) 
            exit(-1); // Call exception TODO
        _astCntrl->crrntBlock->addNewName((*_astCntrl->tokensColl)[_astCntrl->crrntTk++]->phr);
        

        if ((*_astCntrl->tokensColl)[_astCntrl->crrntTk]->id != TOKEN_EQUAL)

            _rtr->add(
                parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithOutAssign(_astCntrl, _typeInfoPos, _declId)
            );


        else {

            _astCntrl->crrntTk++;

            // Memory leak in DataLL // TODO 
            utils::LinkedList<parser::Ast_Node *>* _ = _astCntrl->getNewNodes(0);
            
            _rtr->add(
                parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithAssign(
                    _astCntrl, 
                    _typeInfoPos,
                    _declId,
                    !_->count ? NULL : _->frst->object
                )
            );

            free(_);
        
        }

        if ((*_astCntrl->tokensColl)[_astCntrl->crrntTk]->id == TOKEN_COMMA) {
            _astCntrl->crrntTk++;
            _pntrLevel = 0; _rfrnLevel = 0;
            parser_helper::setPntrRfrnLevel(_astCntrl, &_pntrLevel, &_rfrnLevel);
            new(_type) TypeInformation(_type->userDefined, _type->typeId, _pntrLevel, _rfrnLevel); // Handle in getNewNode()  to be malloc as well
        }        

    }   
    // if (!_rtr->count) new Ast_Control_Exception("Expected token identifier after Type indentifier"); Error dont know what
    return _rtr;
}




