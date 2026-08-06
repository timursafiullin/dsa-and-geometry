#include "../include/data_structures/array.h"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace dsa;

template<typename T, int SIZE>
void printArray(const array<T, SIZE>& arr, const std::string& label)
{
    std::cout << label << ": [";

    for (int i = 0; i < SIZE; ++i)
    {
        std::cout << arr[i];

        if (i != SIZE - 1)
            std::cout << ", ";
    }

    std::cout << "]\n";
}

int main()
{
    constexpr int size = 5;

    array<int, size> myarray;

    std::cout << "=== dsa::array demo ===\n\n";

    std::cout << "1. Fill array with 5\n";
    myarray.fill(5);
    printArray(myarray, "Array");

    std::cout << "\n2. Change element at index 2 to 10\n";
    myarray[2] = 10;
    printArray(myarray, "Array");

    std::cout << "\n3. Insert 7 at index 1\n";
    std::cout << "   Elements are shifted to the right.\n";
    std::cout << "   The last element is discarded.\n";

    myarray.insert(1, 7);
    printArray(myarray, "Array");

    std::cout << "\n4. Insert 42 at index 0\n";
    myarray.insert(0, 42);
    printArray(myarray, "Array");

    std::cout << "\n5. Remove element at index 2\n";
    std::cout << "   Elements are shifted to the left.\n";
    std::cout << "   The last element is replaced with -1.\n";

    myarray.remove(2, -1);
    printArray(myarray, "Array");

    std::cout << "\n6. Remove the first element\n";
    myarray.remove(0, 0);
    printArray(myarray, "Array");

    std::cout << "\n7. Read individual elements\n";

    for (int i = 0; i < size; ++i)
    {
        std::cout << "   array[" << i << "] = " << myarray[i] << '\n';
    }

    std::cout << "\n8. Try invalid insertion\n";

    try
    {
        myarray.insert(size, 100);
    }
    catch (const std::out_of_range& exception)
    {
        std::cout << "   Caught exception: " << exception.what() << '\n';
    }

    std::cout << "\n=== Demo finished ===\n";

    return 0;
}
