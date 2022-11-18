#pragma once

#ifndef VM_H
#define VM_h

namespace parser { struct Byte_Code_Converter_Control; }

namespace vm {

    struct Vm {

        parser::Byte_Code_Converter_Control* bcCnvrCntrl;

        Vm(parser::Byte_Code_Converter_Control*);

        void execute();

    };


}

#endif