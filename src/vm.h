#ifndef VM_H
#define VM_H

#include <vector>
#include <stack>
#include <memory>
#include <initializer_list>

#include "instr.h"
#include "value.h"
#include "function.h"
#include "op_codes.h"

class Vm {
public:
    explicit Vm(Function* function);

    void execute(std::initializer_list<Value> args);
private:
    struct ActivationRecord {
        ActivationRecord(instr_t* pc, Function* function, Value* registers)
            : pc(pc), function(function), registers(registers) {};

        instr_t* pc;
        Function* function;
        std::unique_ptr<Value[]> registers;
    };

    std::stack<ActivationRecord> records;

    Value* constants;
    Value* registers;
    instr_t* pc;

    template <OpCode Op>
    inline void handle(instr_t instruction);

    void call(Function* function);
    void activate(const ActivationRecord& ar);
};

#endif
