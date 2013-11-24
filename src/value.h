#ifndef VALUE_H
#define VALUE_H

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <ostream>

#include "variant.h"

class Function;

//using Value = std::int32_t;

using number_t = double;

class Value : private Variant<std::nullptr_t, bool, number_t, void*> {
public:
    static Value none() { return Value(); }
    static Value boolean(bool value) { return Value(Type::BOOL, value); }
    static Value number(number_t value) { return Value(Type::NUMBER, value); }
    static Value function(Function* function) { return Value(Type::FUNCTION, function); }

    Value() : type(Type::NONE) {}

    Value operator+(const Value& rhs) const {
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return Value::number(get<number_t>() + rhs.get<number_t>());
    }
    Value operator-(const Value& rhs) const {
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return Value::number(get<number_t>() - rhs.get<number_t>());
    }
    Value operator*(const Value& rhs) const { return Value(); }
    Value operator/(const Value& rhs) const { return Value(); }

    bool operator==(const Value& rhs) const {
        // TODO: Support bool
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return get<number_t>() == rhs.get<number_t>();
    }
    bool operator<(const Value& rhs) const {
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return get<number_t>() < rhs.get<number_t>();
    }
    bool operator<=(const Value& rhs) const {
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return get<number_t>() < rhs.get<number_t>();
    }

    operator Function*() {
        assert(type == Type::FUNCTION);
        return static_cast<Function*>(get<void*>());
    }
private:
    friend std::ostream& operator<<(std::ostream& os, const Value& value);

    using Variant::get;

    enum class Type : char {
        NONE,
        BOOL,
        NUMBER,
        FUNCTION,
    } type;

    template <typename T>
    Value(Type type, T value) : Variant(value), type(type) {}
};

#endif
