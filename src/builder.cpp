#include "instr.h"
#include "function.h"
#include "native_types.h"

#include "builder.h"

Register FunctionBuilder::create_register() {
    return register_count++;
}

Constant FunctionBuilder::create_constant(const Object& value) {
    constants.push_back(value);
    return k(constants.size() - 1);
}

Constant FunctionBuilder::self() {
    if (self_ref == -1) {
        self_ref = create_constant(NoneType::create());
    }

    return self_ref;
}

Function* FunctionBuilder::build(instr_t* instructions) {
    if (function.get() != nullptr) {
        return function.get();
    }

    function = std::unique_ptr<Function>(new Function(register_count, constants.data(), instructions));

    if (self_ref != -1) {
        constants[index_k(self_ref)] = FunctionType::create(function.get());
    }

    return function.get();
}