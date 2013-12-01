#ifndef BUILDER_H
#define BUILDER_H

#include <cstdint>

#include <vector>
#include <memory>

#include "type.h"

class Function;

using Register = std::uint8_t;
using Constant = std::uint16_t;

class FunctionBuilder {
public:
    Register create_register();
    Constant create_constant(const Object& value);
    Constant self();

    Function* build(instr_t* instructions);
private:
    int self_ref = -1;

    std::size_t register_count = 0;
    std::vector<Object> constants;

    std::unique_ptr<Function> function;
};

#endif
