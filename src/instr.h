#ifndef INSTR_H
#define INSTR_H

#include <climits>
#include <limits>
#include <cstdint>

#include <iostream>

typedef std::uint32_t instr_t;
typedef std::uint8_t op_t;

const std::size_t INSTRUCTION_SIZE = sizeof(instr_t);

const std::size_t OP_SIZE = 6;
const std::size_t OP_OFFSET = INSTRUCTION_SIZE * CHAR_BIT - OP_SIZE;

const std::size_t A_SIZE = 8;
const std::size_t A_OFFSET = OP_OFFSET - A_SIZE;

const std::size_t B_SIZE = 9;
const std::size_t B_OFFSET = A_OFFSET - B_SIZE;

const std::size_t C_SIZE = 9;
const std::size_t C_OFFSET = B_OFFSET - C_SIZE;

const std::size_t BSS_SIZE = 26;
const std::size_t BSS_OFFSET = OP_OFFSET - BSS_SIZE;

const std::size_t BUU_MAX_VALUE = 0x3ffffff; // TODO: Use bit_mask

const std::uint32_t CONSTANT_BIT = 1 << 8;

constexpr std::uint32_t bit_mask(int x) {
    return (1U << x) - 1;
}

constexpr std::uint32_t to_bss(std::int32_t value) {
    return static_cast<std::uint32_t>(value) + BUU_MAX_VALUE / 2;
}

constexpr std::int32_t from_bss(std::uint32_t value) {
    return static_cast<std::int32_t>(value - BUU_MAX_VALUE / 2);
}

template <typename T>
constexpr instr_t instr_abc(T op, std::uint8_t a = 0, std::uint16_t b = 0, std::uint16_t c = 0) {
    return static_cast<op_t>(op) << OP_OFFSET
        | a << A_OFFSET
        | b << B_OFFSET
        | c << C_OFFSET;
}

template <typename T>
constexpr instr_t instr_bss(T op, std::int32_t bss) {
    return static_cast<op_t>(op) << OP_OFFSET
        | to_bss(bss) << BSS_OFFSET;
}

constexpr std::size_t r(std::size_t i) { return i; }
constexpr std::uint32_t k(std::uint8_t value) { return value | CONSTANT_BIT; }

inline op_t get_op(instr_t instruction) {
    return instruction >> OP_OFFSET & bit_mask(OP_SIZE);
}

inline std::uint8_t get_a(instr_t instruction) {
    return instruction >> A_OFFSET & bit_mask(A_SIZE);
}

inline std::uint32_t get_b(instr_t instruction) {
    return instruction >> B_OFFSET & bit_mask(B_SIZE);
}

inline std::uint32_t get_c(instr_t instruction) {
    return instruction >> C_OFFSET & bit_mask(C_SIZE);
}

inline std::int32_t get_bss(instr_t instruction) {
    return from_bss(instruction >> BSS_OFFSET & bit_mask(BSS_SIZE));
}

inline bool is_k(std::uint32_t index) { return index & CONSTANT_BIT; }
inline std::size_t index_k(std::uint32_t index) { return index & ~CONSTANT_BIT; }

#endif
