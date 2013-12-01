#ifndef FUNCTION_H
#define FUNCTION_H

#include "instr.h"

class Object;

class Function {
public:
    Function(std::size_t _reg_count, Object* _constants, instr_t* _instructions)
            : reg_count(_reg_count), m_constants(_constants), instructions(_instructions) {}

    std::size_t register_count() const { return reg_count; }
    Object* constants() const { return m_constants; }
    instr_t* entry_point() const { return instructions; }
private:
    std::size_t reg_count;
    Object* m_constants;
    instr_t* instructions;
};

#endif
