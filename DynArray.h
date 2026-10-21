#pragma once
#include <stdexcept>
#include "Utils.h"

// Class template: custom dynamic container on the heap (new[] / delete[]).
// Implements the Rule of Three (destructor, copy ctor, copy assignment).

// Function template
template <typename T>
void swapValues(T& a, T& b) { T tmp = a; a = b; b = tmp; }

template <typename T>
class DynArray {
private:
    T* data;
    int sz;
    int cap;

    void grow() {
        int newCap = cap * 2;
        T* nd = new T[newCap];
        for (int i = 0; i < sz; ++i) nd[i] = data[i];
        delete[] data;
        data = nd;
        cap = newCap;
    }

public:
    explicit DynArray(int c = 8) : data(new T[c > 0 ? c : 1]), sz(0), cap(c > 0 ? c : 1) {}

    // Copy constructor (deep copy)
    DynArray(const DynArray& o) : data(new T[o.cap]), sz(o.sz), cap(o.cap) {
        for (int i = 0; i < sz; ++i) data[i] = o.data[i];
    }

    // Destructor
    ~DynArray() { delete[] data; }

    // Copy assignment (deep copy, self-assignment safe)
    DynArray& operator=(const DynArray& o) {
        if (this != &o) {
            T* nd = new T[o.cap];
            for (int i = 0; i < o.sz; ++i) nd[i] = o.data[i];
            delete[] data;
            data = nd;
            sz = o.sz;
            cap = o.cap;
        }
        return *this;
    }

    void push_back(const T& v) {
        if (sz == cap) grow();
        data[sz++] = v;
    }
    DynArray& operator+=(const T& v) { push_back(v); return *this; }

    T pop_back() {
        if (sz == 0) throw std::out_of_range("DynArray::pop_back on empty array");
        return data[--sz];
    }

    void insertAt(int i, const T& v) {
        if (i < 0 || i > sz) throw std::out_of_range("DynArray::insertAt");
        if (sz == cap) grow();
        for (int k = sz; k > i; --k) data[k] = data[k - 1];
        data[i] = v;
        ++sz;
    }

    T removeAt(int i) {
        if (i < 0 || i >= sz) throw std::out_of_range("DynArray::removeAt");
        T v = data[i];
        for (int k = i; k < sz - 1; ++k) data[k] = data[k + 1];
        --sz;
        return v;
    }

    T& operator[](int i) {
        if (i < 0 || i >= sz) throw std::out_of_range("DynArray index");
        return data[i];
    }
    const T& operator[](int i) const {
        if (i < 0 || i >= sz) throw std::out_of_range("DynArray index");
        return data[i];
    }

    int size() const { return sz; }
    bool empty() const { return sz == 0; }
    void clear() { sz = 0; }
};

// Function template: Fisher-Yates shuffle
template <typename T>
void shuffleArray(DynArray<T>& arr) {
    for (int i = arr.size() - 1; i > 0; --i)
        swapValues(arr[i], arr[randInt(i + 1)]);
}
