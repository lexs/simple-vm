#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <ostream>

#include "value.h"

class Array {
public:
    Array() {}
    Array(std::initializer_list<Value> l) : array(l) {} 

    std::size_t size() const {
        return array.size();
    }

    const Value& operator[](std::size_t index) const {
        return array[index];
    }

    Value& operator[](std::size_t index) {
        if (size() <= index) array.resize(index + 1);
        return array[index];
    }

    std::vector<Value>::const_iterator begin() const {
        return array.begin();
    }

    std::vector<Value>::const_iterator end() const {
        return array.end();
    }
private:
    std::vector<Value> array;
};

std::ostream& operator<<(std::ostream& os, const Array& array);

#endif
