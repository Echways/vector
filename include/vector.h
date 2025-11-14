#include <iostream>
#include <memory>

template<typename T>
class Vector{
private:
    T* arr;
    size_t sz;
    size_t cap;

public:
    bool reserve(size_t);
    bool resize(size_t, const T& = T());
    bool push_back(const T&);
    bool pop_back();
    T& operator[](size_t);
    const T& operator[](size_t) const;
    T& at(size_t);

    Vector();
    Vector(size_t);
    ~Vector();

    size_t size() const;
    size_t capacity() const;
};