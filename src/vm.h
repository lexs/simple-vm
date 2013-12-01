#ifndef VM_H
#define VM_H

#include <vector>
#include <stack>
#include <memory>
#include <initializer_list>

#include "instr.h"
#include "type.h"
#include "function.h"
#include "op_codes.h"

class Vm {
public:
    explicit Vm(Function* function);

    void execute(std::initializer_list<Object> args);
private:
    struct ActivationRecord {
        ActivationRecord(instr_t* pc, Function* function, Object* registers)
            : pc(pc), function(function), registers(registers) {};

        instr_t* pc;
        Function* function;
        std::unique_ptr<Object[]> registers;
    };

    std::stack<ActivationRecord> records;

    Object* constants;
    Object* registers;
    instr_t* pc;

    template <OpCode Op>
    inline void handle(instr_t instruction);

    void call(Function* function);
    void activate(const ActivationRecord& ar);
};

#endif
