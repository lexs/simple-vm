#include <cassert>
#include <iostream>
#include <functional>

#include "array.h"
#include "op_codes.h"
#include "vm.h"

// http://www.parashift.com/c++-faq/macro-for-ptr-to-memfn.html
#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

#define R_A(i) registers[get_a(i)]
#define R_B(i) registers[get_b(i)]
#define R_C(i) registers[get_c(i)]

#define RK_(index) (is_k(index) ? constants[index_k(index)] : registers[index])

#define RK_B(i) RK_(get_b(i))
#define RK_C(i) RK_(get_c(i))

Vm::Vm(Function* function) : pc(nullptr) {
    call(function);
}

template <>
void Vm::handle<OpCode::NOP>(instr_t i) {
}

template <>
void Vm::handle<OpCode::MOVE>(instr_t i) {
    R_A(i) = R_B(i);
}

template <>
void Vm::handle<OpCode::LOADK>(instr_t i) {
    R_A(i) = constants[get_b(i)];
}

template <>
void Vm::handle<OpCode::JUMP>(instr_t i) {
    pc += get_bss(i);
}

template <>
void Vm::handle<OpCode::ADD>(instr_t i) {
    R_A(i) = RK_B(i) + RK_C(i);
}

template <>
void Vm::handle<OpCode::SUB>(instr_t i) {
    R_A(i) = RK_B(i) - RK_C(i);
}

template <>
void Vm::handle<OpCode::MUL>(instr_t i) {
    R_A(i) = RK_B(i) * RK_C(i);
}

template <>
void Vm::handle<OpCode::DIV>(instr_t i) {
    R_A(i) = RK_B(i) / RK_C(i);
}

template <>
void Vm::handle<OpCode::EQ>(instr_t i) {
    if (R_A(i) == RK_B(i)) {
        assert(get_op(*pc) == (op_t) OpCode::JUMP);
        handle<OpCode::JUMP>(*pc++);
    } else {
        pc++;
    }
}

template <>
void Vm::handle<OpCode::LT>(instr_t i) {
    if (R_A(i) < RK_B(i)) {
        assert(get_op(*pc) == (op_t) OpCode::JUMP);
        handle<OpCode::JUMP>(*pc++);
    } else {
        pc++;
    }
}

template <>
void Vm::handle<OpCode::LE>(instr_t i) {
    if (R_A(i) <= RK_B(i)) {
        assert(get_op(*pc) == (op_t) OpCode::JUMP);
        handle<OpCode::JUMP>(*pc++);
    } else {
        pc++;
    }
}

template <>
void Vm::handle<OpCode::NEW_ARRAY>(instr_t i) {
    R_A(i) = Value::array(std::make_shared<Array>());
}

template <>
void Vm::handle<OpCode::SET>(instr_t i) {
    Array& a = R_A(i);

    std::size_t index = static_cast<std::size_t>(RK_B(i));
    a[index] = RK_C(i);
}

template <>
void Vm::handle<OpCode::GET>(instr_t i) {
    Array& a = R_B(i);
    std::size_t index = static_cast<std::size_t>(RK_C(i));
    R_A(i) = a[index];
}

template <>
void Vm::handle<OpCode::LEN>(instr_t i) {
    Array& a = RK_B(i);
    R_A(i) = Value::number(a.size());
}

template <>
void Vm::handle<OpCode::CALL>(instr_t i) {
    Function* function = R_A(i);
    std::size_t args_start = get_b(i);
    std::size_t args_count = get_c(i);

    // TODO: Know arg count
    assert(args_count <= function->register_count());

    Value* calling_reg = registers;

    call(function);

    // Copy arguments
    std::copy(&calling_reg[args_start], &calling_reg[args_start + args_count], registers);
}

template <>
void Vm::handle<OpCode::RET>(instr_t i) {
    ActivationRecord& ar = records.top();

    std::unique_ptr<Value[]> old_registers = std::move(ar.registers);
    pc = ar.pc;

    records.pop();

    if (!records.empty()) {
        std::uint8_t return_start = get_a(i);
        std::uint8_t return_count = get_b(i);

        instr_t call_i = *(pc - 1);
        std::uint8_t result_start = get_b(call_i);
        std::uint8_t result_count = get_c(call_i);
        
        assert(result_count <= return_count);
        std::uint8_t count = std::min(return_count, result_count);

        activate(records.top());

        std::copy(&old_registers[return_start], &old_registers[return_start + count], &registers[result_start]);
    } else {
        assert(pc == nullptr);
    }
}

template <>
void Vm::handle<OpCode::PRINT>(instr_t i) {
    std::cout << "R(" << (int) get_a(i) << ") = " << R_A(i) << std::endl;
}

void Vm::call(Function* function) {
    records.emplace(pc, function, new Value[function->register_count()]);
    pc = function->entry_point();
    activate(records.top());
}

void Vm::activate(const ActivationRecord& ar) {
    registers = ar.registers.get();
    constants = ar.function->constants();
}

void Vm::execute(std::initializer_list<Value> args) {
    std::copy(args.begin(), args.end(), registers);

    typedef void (Vm::*fn)(instr_t);

    fn op_table[] = {
        &Vm::handle<OpCode::NOP>,
        &Vm::handle<OpCode::MOVE>,
        &Vm::handle<OpCode::LOADK>,
        &Vm::handle<OpCode::JUMP>,
        &Vm::handle<OpCode::ADD>,
        &Vm::handle<OpCode::SUB>,
        &Vm::handle<OpCode::MUL>,
        &Vm::handle<OpCode::DIV>,
        &Vm::handle<OpCode::EQ>,
        &Vm::handle<OpCode::LT>,
        &Vm::handle<OpCode::LE>,
        &Vm::handle<OpCode::NEW_ARRAY>,
        &Vm::handle<OpCode::SET>,
        &Vm::handle<OpCode::GET>,
        &Vm::handle<OpCode::LEN>,
        &Vm::handle<OpCode::CALL>,
        &Vm::handle<OpCode::RET>,
        &Vm::handle<OpCode::PRINT>
    };

    while (pc) {
        instr_t i = *pc++;
        CALL_MEMBER_FN(*this, op_table[get_op(i)])(i);
    }
}
