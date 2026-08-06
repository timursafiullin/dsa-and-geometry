#pragma once

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <stdexcept>

#define _DEFAULT_CAPACITY 8

namespace dsa
{

template<typename T>
class vector
{
public:
    explicit vector(int size);

    vector(std::initializer_list<T> list);

    ~vector();

    vector(const vector& obj);
    vector& operator=(const vector& obj);

    vector(vector&& obj);
    vector& operator=(vector&& obj);

    T& operator[](int idx);
    T operator[](int idx) const;

    int size() const { return _size; }
    int capacity() const { return _capacity; }

    void reserve(int capacity);
    void resize(int new_size);

    void push_back(T value);

private:
    int _size;
    int _capacity;
    T* _elements;
};


// MARK: Constructors
template<typename T>
vector<T>::vector(int size)
    : _size(size), _capacity(size), _elements(nullptr)
{
    if (size < 0)
        throw std::invalid_argument("Vector size cannot be negative.");

    _elements = new T[size];
    for (int i = 0; i < size; ++i)
        _elements[i] = T{};
}

template<typename T>
vector<T>::vector(std::initializer_list<T> list)
    : _size(list.size()), _capacity(list.size()), _elements(new T[list.size()])
{
    std::copy(list.begin(), list.end(), _elements);
}


// Destructor
template<typename T>
vector<T>::~vector() { delete[] _elements; }

// MARK: Copying
template<typename T>
vector<T>::vector(const vector<T>& obj)
    : _size(obj.size()), _capacity(obj.capacity()), _elements(new T[obj.capacity()])
{
    for (int i = 0; i < obj.size(); ++i)
        _elements[i] = obj._elements[i];
}

template<typename T>
vector<T>& vector<T>::operator=(const vector<T>& obj)
{
    if (this == &obj) return *this;

    if (obj.size() <= _capacity)
    {
        for (int i = 0; i < obj.size(); ++i)
            _elements[i] = obj._elements[i];

        _size = obj.size();

        return *this;
    }

    T* pointer = new T[obj.capacity()];

    for (int i = 0; i < obj.size(); ++i)
        pointer[i] = obj._elements[i];

    for (int i = obj.size(); i < obj.capacity(); ++i)
        pointer[i] = T{};

    delete[] _elements;

    _size = obj.size();
    _capacity = obj.capacity();
    _elements = pointer;
    
    return *this;
}


// MARK: Moving
template<typename T>
vector<T>::vector(vector<T>&& obj)
    : _size(obj.size()), _capacity(obj.capacity()), _elements(obj._elements)
{

    obj._elements = nullptr;
    obj._size = 0;
    obj._capacity = 0;
}

template<typename T>
vector<T>& vector<T>::operator=(vector<T>&& obj)
{
    if (this == &obj)
        return *this;

    delete[] _elements;

    _elements = obj._elements;
    _size = obj.size();
    _capacity = obj.capacity();

    obj._elements = nullptr;
    obj._size = 0;
    obj._capacity = 0;

    return *this;
}


// MARK: Access
template<typename T>
T& vector<T>::operator[](int idx)
{
    if (idx < -_size || idx >= _size)
        throw std::out_of_range("Vector index is out of range.");

    if (idx < 0)
        return _elements[_size + idx];

    return _elements[idx]; 
}

template<typename T>
T vector<T>::operator[](int idx) const
{
    if (idx < -_size || idx >= _size)
        throw std::out_of_range("Vector index is out of range.");

    if (idx < 0)
        return _elements[_size + idx];

    return _elements[idx];
}


// Data Size
template<typename T>
void vector<T>::reserve(int capacity)
{
    if (capacity < 0)
        throw std::invalid_argument("Vector capacity cannot be negative.");

    if (capacity <= _capacity) return;

    T* pointer = new T[capacity];
    for (int i = 0; i < _size; ++i) pointer[i] = _elements[i];

    delete[] _elements;

    _elements = pointer;
    _capacity = capacity;
}

template<typename T>
void vector<T>::resize(int new_size)
{
    if (new_size < 0)
        throw std::invalid_argument("Vector size cannot be negative.");

    reserve(new_size);
    for (int i = _size; i < new_size; ++i) _elements[i] = T{};
    _size = new_size;
}


template<typename T>
void vector<T>::push_back(T value)
{
    if (_capacity == 0)
        reserve(_DEFAULT_CAPACITY);
    else if (_size == _capacity)
        reserve(_capacity * 2);
    
    _elements[_size] = value;
    ++_size;
}

}
