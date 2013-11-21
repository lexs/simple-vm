#ifndef FUNCTION_H
#define FUNCTION_H

#include "instr.h"

class Function {
public:
    Function(std::size_t _reg_count, Value* _constants, instr_t* _instructions)
            : reg_count(_reg_count), m_constants(_constants), instructions(_instructions) {}

    std::size_t register_count() const { return reg_count; }
    Value* constants() const { return m_constants; }
    instr_t* entry_point() const { return instructions; }
private:
    std::size_t reg_count;
    Value* m_constants;
    instr_t* instructions;
};

#endif
