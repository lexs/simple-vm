#ifndef NATIVE_TYPES_H
#define NATIVE_TYPES_H

#include <memory>

#include "singleton.h"
#include "array.h"
#include "type.h"

class Function;

struct NoneType : public Type, Singleton<NoneType> {
    static Object create() {
        return Object(&instance(), nullptr);
    }

    int compare(const Value& a, const Value& b) override {
        return 0;
    }

    void print(std::ostream& os, const Value& value) override {
        os << "None";
    }
};

struct BoolType : public Type, Singleton<BoolType> {
    static Object create(bool value) {
        return Object(&instance(), value);
    }

    int compare(const Value& a, const Value& b) override {
        bool lhs = a.get<bool>();
        bool rhs = b.get<bool>();
        return lhs == rhs ? 0 : lhs ? 1 : -1;
    }

    void print(std::ostream& os, const Value& value) override {
        os << (value.get<bool>() ? "true" : "false");
    }
};

struct NumberType : public Numeric, Singleton<NumberType> {
    static Object create(number_t value) {
        return Object(&instance(), value);
    }

    int compare(const Value& a, const Value& b) override {
        return a.get<number_t>() - b.get<number_t>();
    }

    void print(std::ostream& os, const Value& value) override {
        os << value.get<number_t>();
    }

    int int_value(const Value& value) override {
        return value.get<number_t>();
    }

    Object add(const Value& a, const Value& b) override {
        return Object(this, a.get<number_t>() + b.get<number_t>());
    }

    Object sub(const Value& a, const Value& b) override {
        return Object(this, a.get<number_t>() - b.get<number_t>());
    }

    Object mul(const Value& a, const Value& b) override {
        return Object(this, a.get<number_t>() * b.get<number_t>());
    }

    Object div(const Value& a, const Value& b) override {
        return Object(this, a.get<number_t>() / b.get<number_t>());
    }
};

struct FunctionType : public Type, Singleton<FunctionType> {
    static Object create(Function* function) {
        return Object(&instance(), function);
    }

    static Function* get(Object& o) {
        assert(o.type() == &instance());
        return static_cast<Function*>(o.value().get<void*>());
    }

    int compare(const Value& a, const Value& b) override {
        void* lhs = a.get<void*>();
        void* rhs = b.get<void*>();
        return lhs == rhs ? 0 : lhs < rhs ? -1 : 1;
    }

    void print(std::ostream& os, const Value& value) override {
        os << "<function at " << value.get<void*>() << ">";
    }
};

struct ArrayType : public Sequence, Singleton<ArrayType> {
    static Object create(const std::shared_ptr<Array>& array) {
        return Object(&instance(), array);
    }

    int compare(const Value& a, const Value& b) override {
        return 0;
    }

    void print(std::ostream& os, const Value& value) override {
        os << get(value);
    }

    std::size_t size(Value& sequence) override {
        return get(sequence).size();
    }

    Object& get(Value& sequence, std::size_t index) override {
        return get(sequence)[index];
    }

    void set(Value& sequence, std::size_t index, const Object& value) override {
        get(sequence)[index] = value;
    }
private:
    Array& get(Value& value) {
        return *static_cast<Array*>(value.get<std::shared_ptr<void>>().get());
    }

    const Array& get(const Value& value) {
        return *static_cast<const Array*>(value.get<std::shared_ptr<void>>().get());
    }
};

#endif
