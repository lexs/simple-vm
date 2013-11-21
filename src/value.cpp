#include "value.h"

std::ostream& operator<<(std::ostream& os, const Value& value) {
    using Type = Value::Type;

    switch (value.type) {
        case Type::NONE:
            return os << "None";
        case Type::BOOL:
            return os << (value.v.boolean ? "true" : "false");
        case Type::NUMBER:
            return os << value.v.number;
        case Type::FUNCTION:
            return os << "<function at " << value.v.data << ">";
        default:
            assert(false);
            return os;
    }
}
