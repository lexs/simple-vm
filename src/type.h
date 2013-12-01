#ifndef TYPE_H
#define TYPE_H

#include <ostream>
#include <cstddef>
#include <cassert>
#include <utility>

#include "variant.h"

using number_t = std::int32_t;
using Value = Variant<std::nullptr_t, bool, number_t, void*, std::shared_ptr<void>>;

class Type;
class Numeric;
class Sequence;

class Object {
public:
    Object();

    template <typename T>
    Object(Type* type, T&& value) : m_type(type), m_value(std::forward<T>(value)) {}

    Type* type() const { return m_type; }
    const Value& value() const { return m_value; }
    Value& value() { return m_value; }
private:
    Type* m_type;
    Value m_value;
};

bool operator==(const Object& lhs, const Object& rhs);
bool operator<(const Object& lhs, const Object& rhs);
bool operator<=(const Object& lhs, const Object& rhs);

Object operator+(const Object& a, const Object& b);
Object operator-(const Object& a, const Object& b);
Object operator*(const Object& a, const Object& b);
Object operator/(const Object& a, const Object& b);

std::ostream& operator<<(std::ostream& os, const Object& object);

class Type {
public:
    virtual Numeric* as_numeric() {
        return nullptr;
    }

    virtual Sequence* as_sequence() {
        return nullptr;
    }

    //virtual Value join(const std::vector<Value>& values) = 0;

    virtual int compare(const Value& a, const Value& b) = 0;

    virtual void print(std::ostream& os, const Value& value) = 0;
};

class Numeric : public Type {
public:
    static int convert(Object& o) {
        assert(o.type()->as_numeric() != nullptr);
        return o.type()->as_numeric()->int_value(o.value());
    }

    Numeric* as_numeric() override {
        return this;
    }

    virtual int int_value(const Value& value) = 0;

    virtual Object add(const Value& a, const Value& b) = 0;
    virtual Object sub(const Value& a, const Value& b) = 0;
    virtual Object mul(const Value& a, const Value& b) = 0;
    virtual Object div(const Value& a, const Value& b) = 0;
};

class Sequence : public Type {
public:
    static Object& get(Object& o, std::size_t index) {
        assert(o.type()->as_sequence() != nullptr);
        return o.type()->as_sequence()->get(o.value(), index);
    }

    static void set(Object& o, std::size_t index, const Object& value) {
        assert(o.type()->as_sequence() != nullptr);
        o.type()->as_sequence()->set(o.value(), index, value);
    }

    Sequence* as_sequence() override {
        return this;
    }

    virtual std::size_t size(Value& sequence) = 0;
    virtual Object& get(Value& sequence, std::size_t index) = 0;
    virtual void set(Value& sequence, std::size_t index, const Object& value) = 0;
};

#endif
