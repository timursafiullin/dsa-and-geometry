#pragma once

#include <stdexcept>

namespace dsa
{

template<typename T, int SIZE>
class array
{
public:
    T& operator [](size_t idx)
    {
        return _array[idx];
    }

    T operator [](size_t idx) const
    {
        return _array[idx];
    }

    void fill(const T& value)
    {
        for (size_t i = 0; i < SIZE; ++i)
            _array[i] = value;
    }

    /**
     * Inserts the index value. Shifts the elements to the right, and the last element is discarded.
     *
     * @param idx Element index.
     * @throws std::out_of_range If you have gone beyond the boundaries.
     */
    void insert(size_t idx, T obj)
    {
        if (idx >= SIZE)
            throw std::out_of_range("Index out of range.");

        for (size_t i = SIZE - 1; i > idx; --i)
            this->operator[](i) = this->operator[](i - 1);

        this->operator[](idx) = obj;
    }

    /**
     * Deletes an element by index. Shifts the elements from right to left, the last element is filled with a replacement value.
     *
     * @param idx Element index.
     * @param replacement Replacement value.
     * @throws std::out_of_range If you have gone beyond the boundaries.
     */
    void remove(size_t idx, const T& replacement)
    {
        if (idx >= SIZE)
            throw std::out_of_range("Index out of range.");

        for (size_t i = idx; i < SIZE - 1; ++i)
            this->operator[](i) = this->operator[](i + 1);
        
        this->operator[](SIZE - 1) = replacement;
    }

private:
    T _array[SIZE];
};

}