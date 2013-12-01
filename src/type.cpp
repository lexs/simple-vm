#include "type.h"
#include "native_types.h"

namespace {
    inline void assert_numeric(const Object& a, const Object& b) {
        assert(a.type() == b.type());
        assert(a.type()->as_numeric() != nullptr);
    }
}

Object::Object() : Object(NoneType::create()) {}

bool operator==(const Object& lhs, const Object& rhs) {
    assert(lhs.type() == rhs.type());
    return lhs.type()->compare(lhs.value(), rhs.value()) == 0;
}
bool operator<(const Object& lhs, const Object& rhs) {
    assert(lhs.type() == rhs.type());
    return lhs.type()->compare(lhs.value(), rhs.value()) < 0;
}
bool operator<=(const Object& lhs, const Object& rhs) {
    assert(lhs.type() == rhs.type());
    return lhs.type()->compare(lhs.value(), rhs.value()) <= 0;
}

Object operator+(const Object& a, const Object& b) {
    // TODO: Fallback to concat for sequence type
    assert_numeric(a, b);
    return a.type()->as_numeric()->add(a.value(), b.value());
}

Object operator-(const Object& a, const Object& b) {
    assert_numeric(a, b);
    return a.type()->as_numeric()->sub(a.value(), b.value());
}

Object operator*(const Object& a, const Object& b) {
    assert_numeric(a, b);
    return a.type()->as_numeric()->mul(a.value(), b.value());
}

Object operator/(const Object& a, const Object& b) {
    assert_numeric(a, b);
    return a.type()->as_numeric()->div(a.value(), b.value());
}

std::ostream& operator<<(std::ostream& os, const Object& object) {
    object.type()->print(os, object.value());
    return os;
}