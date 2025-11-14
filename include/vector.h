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

template<class T>
bool Vector<T>::reserve(size_t n) {
    if(n <= cap) return 0;
    T* newarr = reinterpret_cast<T*>(new int8_t[n * sizeof(T)]);
    try {
        std::uninitialized_copy(arr, arr+sz, newarr);
    } catch (...) {
        delete[] reinterpret_cast<int8_t*>(newarr);
        throw 1;
    }

    for(size_t i = 0; i < sz; ++i) {
        (arr + i) -> ~T();
    }
    delete[] reinterpret_cast<int8_t*>(arr);
    arr = newarr;
    cap = n;
    return 0;
}; 

template<class T>
bool Vector<T>::resize(size_t n, const T& value) {
    if(n > cap) reserve(n);
    try {
        for(size_t i = sz; i < n; ++i){
            new (arr + i) T(value);
        }
    } catch (...) {
        for(size_t i = sz; i < n; ++i) {
            (arr + i) -> ~T();
        }
        throw 1;
    }
    if(n < sz){
        for(size_t i = n; i < sz; ++i) {
            (arr + i) -> ~T();
        }
    }
    sz = n;
    return 0;
}; 

template<class T>
bool Vector<T>::push_back(const T& value) {
    if(cap == sz){
        reserve(2 * cap);
    }
    try {
        new (arr+sz) T(value);
    } catch (...) {
        throw 1;
    }
    ++sz;
    return 0;
};

template<class T>
bool Vector<T>::pop_back() {
    --sz;
    (arr + sz) -> ~T();
    return 1;
};

template<class T>
T& Vector<T>::operator[](size_t i) {
    return arr[i];
};

template<class T>
const T& Vector<T>::operator[](size_t i) const {
    return arr[i];
};

template<class T>
T& Vector<T>::at(size_t i) {
    if(i >= sz) throw std::out_of_range("position is greater than the sz");
    return arr[i];
};

template<class T>
Vector<T>::Vector() 
    : arr(reinterpret_cast<T*>(new int8_t[sizeof(T)]))
    , sz(0)
    , cap(1)
{}

template<class T>
Vector<T>::Vector(size_t n) 
    : arr(reinterpret_cast<T*>(new int8_t[n * sizeof(T)]))
    , sz(0)
    , cap(n)
{}

template<class T>
Vector<T>::~Vector() {
    for (size_t i = 0; i < sz; ++i){
        (arr + i) -> ~T();
    }
    delete[] reinterpret_cast<int8_t*>(arr);
}

template<class T>
size_t Vector<T>::size() const {
    return sz;
}

template<class T>
size_t Vector<T>::capacity() const {
    return cap;
}