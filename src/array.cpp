#include "array.h"

std::ostream& operator<<(std::ostream& os, const Array& array) {
    os << "[";
    for (std::size_t i = 0, size = array.size(); i < size; i++) {
        os << array[i];
        if (i < size - 1) os << ", ";
    }
    return os << "]";
}
