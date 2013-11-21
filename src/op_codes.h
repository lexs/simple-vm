#ifndef OP_CODES_H
#define OP_CODES_H

#include "instr.h"

#define OP_CODES(_) \
    _(NOP) \
    _(MOVE) \
    _(JUMP) \
    _(ADD) \
    _(SUB) \
    _(MUL) \
    _(DIV) \
    _(EQ) \
    _(RET) \
    _(PRINT) \

enum class OpCode : op_t {
    NOP,
    MOVE,
    LOADK,

    JUMP,

    ADD,
    SUB,
    MUL,
    DIV,

    EQ,
    LT,
    LE,

    CALL,
    RET,

    PRINT
};

constexpr instr_t nop() { return instr_abc(OpCode::NOP); }
constexpr instr_t move(std::uint8_t to, std::uint16_t from) { return instr_abc(OpCode::MOVE, to, from); }

// TODO: Let loadk take bigger indexes into the constants table.
constexpr instr_t loadk(std::uint8_t to, std::uint8_t value) { return instr_abc(OpCode::LOADK, to, value); }

constexpr instr_t jump(std::int32_t offset) { return instr_bss(OpCode::JUMP, offset); }

constexpr instr_t add(std::uint8_t res, std::uint16_t a, std::uint16_t b) { return instr_abc(OpCode::ADD, res, a, b); }
constexpr instr_t sub(std::uint8_t res, std::uint16_t a, std::uint16_t b) { return instr_abc(OpCode::SUB, res, a, b); }
constexpr instr_t mul(std::uint8_t res, std::uint16_t a, std::uint16_t b) { return instr_abc(OpCode::MUL, res, a, b); }
constexpr instr_t div(std::uint8_t res, std::uint16_t a, std::uint16_t b) { return instr_abc(OpCode::DIV, res, a, b); }

constexpr instr_t eq(std::uint16_t a, std::uint16_t b) { return instr_abc(OpCode::EQ, a, b); }
constexpr instr_t lt(std::uint16_t a, std::uint16_t b) { return instr_abc(OpCode::LT, a, b); }
constexpr instr_t le(std::uint16_t a, std::uint16_t b) { return instr_abc(OpCode::LE, a, b); }

constexpr instr_t call(std::uint8_t function, std::uint8_t start, std::uint8_t count) {
    return instr_abc(OpCode::CALL, function, start, count);
}
constexpr instr_t ret(std::uint8_t start = 0, std::uint8_t count = 0) { return instr_abc(OpCode::RET, start, count); }

constexpr instr_t print(std::uint16_t reg) { return instr_abc(OpCode::PRINT, reg); }

#endif
