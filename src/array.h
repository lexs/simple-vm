#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <ostream>

#include "type.h"

class Array {
public:
    Array() {}
    Array(std::initializer_list<Object> l) : array(l) {} 

    std::size_t size() const {
        return array.size();
    }

    const Object& operator[](std::size_t index) const {
        return array[index];
    }

    Object& operator[](std::size_t index) {
        if (size() <= index) array.resize(index + 1);
        return array[index];
    }

    std::vector<Object>::const_iterator begin() const {
        return array.begin();
    }

    std::vector<Object>::const_iterator end() const {
        return array.end();
    }
private:
    std::vector<Object> array;
};

std::ostream& operator<<(std::ostream& os, const Array& array);

#endif
