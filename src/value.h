#ifndef VALUE_H
#define VALUE_H

#include <cstdint>
#include <cassert>
#include <ostream>

class Function;

//using Value = std::int32_t;

using number_t = double;

class Value {
public:
    static Value none() { return Value(); }
    static Value boolean(bool value) { return Value(Type::BOOL, value); }
    static Value number(number_t value) { return Value(Type::NUMBER, value); }
    static Value function(Function* function) { return Value(Type::FUNCTION, function); }

    Value() : type(Type::NONE) {}

    Value operator+(const Value& rhs) const {
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return Value::number(v.number + rhs.v.number);
    }
    Value operator-(const Value& rhs) const {
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return Value::number(v.number - rhs.v.number);
    }
    Value operator*(const Value& rhs) const { return Value(); }
    Value operator/(const Value& rhs) const { return Value(); }

    bool operator==(const Value& rhs) const {
        // TODO: Support bool
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return v.number == rhs.v.number;
    }
    bool operator<(const Value& rhs) const {
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return v.number < rhs.v.number;
    }
    bool operator<=(const Value& rhs) const {
        assert(type == Type::NUMBER && rhs.type == Type::NUMBER);
        return v.number < rhs.v.number;
    }

    operator Function*() {
        assert(type == Type::FUNCTION);
        return static_cast<Function*>(v.data);
    }
private:
    friend std::ostream& operator<<(std::ostream& os, const Value& value);

    enum class Type : char {
        NONE,
        BOOL,
        NUMBER,
        FUNCTION
    } type;

    union Container {
        Container() {}
        Container(bool b) : boolean(b) {}
        Container(number_t n) : number(n) {}
        Container(void* d) : data(d) {}

        bool boolean;
        number_t number;
        void* data;
    } v;

    template <typename T>
    Value(Type t, T value) : type(t), v(value) {}
};

#endif
