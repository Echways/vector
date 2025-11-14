#pragma once

#include <cstddef>
#include <memory>     // std::uninitialized_copy
#include <new>        // placement new
#include <stdexcept>  // std::out_of_range

template<typename T>
class Vector {
private:
    T* arr;
    std::size_t sz;
    std::size_t cap;

public:
    bool reserve(std::size_t);
    bool resize(std::size_t, const T& = T());
    bool push_back(const T&);
    bool pop_back();

    T& operator[](std::size_t);
    const T& operator[](std::size_t) const;
    T& at(std::size_t);

    Vector();
    explicit Vector(std::size_t);
    ~Vector();

    std::size_t size() const;
    std::size_t capacity() const;
};

template<class T>
Vector<T>::Vector()
    : arr(nullptr)
    , sz(0)
    , cap(0)
{}

template<class T>
Vector<T>::Vector(std::size_t n)
    : arr(nullptr)
    , sz(0)
    , cap(0)
{
    if (n == 0) {
        return;
    }

    arr = reinterpret_cast<T*>(new int8_t[n * sizeof(T)]);
    std::size_t i = 0;
    try {
        for (; i < n; ++i) {
            new (arr + i) T();
        }
    } catch (...) {
        for (std::size_t j = 0; j < i; ++j) {
            (arr + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(arr);
        arr = nullptr;
        throw;
    }

    sz  = n;
    cap = n;
}

template<class T>
Vector<T>::~Vector() {
    for (std::size_t i = 0; i < sz; ++i) {
        (arr + i)->~T();
    }
    delete[] reinterpret_cast<int8_t*>(arr);
}

template<class T>
bool Vector<T>::reserve(std::size_t n) {
    if (n <= cap) {
        return false;
    }

    T* newarr = reinterpret_cast<T*>(new int8_t[n * sizeof(T)]);

    try {
        if (sz > 0) {
            std::uninitialized_copy(arr, arr + sz, newarr);
        }
    } catch (...) {
        delete[] reinterpret_cast<int8_t*>(newarr);
        throw;
    }

    for (std::size_t i = 0; i < sz; ++i) {
        (arr + i)->~T();
    }
    delete[] reinterpret_cast<int8_t*>(arr);

    arr = newarr;
    cap = n;
    return true;
}

template<class T>
bool Vector<T>::resize(std::size_t n, const T& value) {
    if (n > cap) {
        reserve(n);
    }

    if (n > sz) {
        std::size_t i = sz;
        try {
            for (; i < n; ++i) {
                new (arr + i) T(value);
            }
        } catch (...) {
            for (std::size_t j = sz; j < i; ++j) {
                (arr + j)->~T();
            }
            throw;
        }
    } else if (n < sz) {
        for (std::size_t i = n; i < sz; ++i) {
            (arr + i)->~T();
        }
    }

    sz = n;
    return true;
}

template<class T>
bool Vector<T>::push_back(const T& value) {
    if (sz == cap) {
        std::size_t newCap = (cap == 0) ? 1 : (cap * 2);
        reserve(newCap);
    }

    new (arr + sz) T(value);
    ++sz;
    return true;
}

template<class T>
bool Vector<T>::pop_back() {
    if (sz == 0) {
        return false;
    }
    --sz;
    (arr + sz)->~T();
    return true;
}

template<class T>
T& Vector<T>::operator[](std::size_t i) {
    return arr[i];
}

template<class T>
const T& Vector<T>::operator[](std::size_t i) const {
    return arr[i];
}

template<class T>
T& Vector<T>::at(std::size_t i) {
    if (i >= sz) {
        throw std::out_of_range("position is greater than the sz");
    }
    return arr[i];
}

template<class T>
std::size_t Vector<T>::size() const {
    return sz;
}

template<class T>
std::size_t Vector<T>::capacity() const {
    return cap;
}
