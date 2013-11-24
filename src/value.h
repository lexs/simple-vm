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

class Value {
public:
    static Value none() { return Value(); }
    static Value boolean(bool value) { return Value(value); }
    static Value number(number_t value) { return Value(value); }
    static Value function(Function* function) { return Value(function); }

    Value() {}

    Value operator+(const Value& rhs) const {
        assert(type() == Type::NUMBER && rhs.type() == Type::NUMBER);
        return Value::number(value.get<number_t>() + rhs.value.get<number_t>());
    }
    Value operator-(const Value& rhs) const {
        assert(type() == Type::NUMBER && rhs.type() == Type::NUMBER);
        return Value::number(value.get<number_t>() - rhs.value.get<number_t>());
    }
    Value operator*(const Value& rhs) const { return Value(); }
    Value operator/(const Value& rhs) const { return Value(); }

    bool operator==(const Value& rhs) const {
        // TODO: Support bool
        assert(type() == Type::NUMBER && rhs.type() == Type::NUMBER);
        return value.get<number_t>() == rhs.value.get<number_t>();
    }
    bool operator<(const Value& rhs) const {
        assert(type() == Type::NUMBER && rhs.type() == Type::NUMBER);
        return value.get<number_t>() < rhs.value.get<number_t>();
    }
    bool operator<=(const Value& rhs) const {
        assert(type() == Type::NUMBER && rhs.type() == Type::NUMBER);
        return value.get<number_t>() < rhs.value.get<number_t>();
    }

    operator Function*() {
        assert(type() == Type::FUNCTION);
        return static_cast<Function*>(value.get<void*>());
    }
private:
    friend std::ostream& operator<<(std::ostream& os, const Value& value);

    enum class Type : char {
        NONE,
        BOOL,
        NUMBER,
        FUNCTION,
    };


    Variant<std::nullptr_t, bool, number_t, void*> value;

    template <typename T>
    Value(T value) : value(value) {}

    Type type() const { return static_cast<Type>(value.which_type()); }
};

#endif
