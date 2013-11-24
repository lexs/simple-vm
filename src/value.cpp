#include "value.h"

std::ostream& operator<<(std::ostream& os, const Value& value) {
    using Type = Value::Type;

    switch (value.type()) {
        case Type::NONE:
            return os << "None";
        case Type::BOOL:
            return os << (value.value.get<bool>() ? "true" : "false");
        case Type::NUMBER:
            return os << value.value.get<number_t>();
        case Type::FUNCTION:
            return os << "<function at " << value.value.get<void*>() << ">";
        default:
            assert(false);
            return os;
    }
}
