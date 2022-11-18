#include "./vm.h"

#include "./../parser/byteCodeConverter.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./frame.h"

#include <iostream>

vm::Vm::Vm(parser::Byte_Code_Converter_Control* _bcCnvrCntrl) : bcCnvrCntrl(_bcCnvrCntrl) {}

// First execute all global instructions 
// Then execute given contract function
void vm::Vm::execute() {

    vm::Vm_Frame* globalFrame = new vm::Vm_Frame(
        bcCnvrCntrl->byteCodeBlocks->last->object->byteCode,
        bcCnvrCntrl->storage,
        NULL
    );

    globalFrame->execute();

}

