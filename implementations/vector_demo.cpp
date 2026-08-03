#include "vector.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace my;

template<typename T>
void printVector(const vector<T>& values, const std::string& label)
{
    std::cout << label << " (size = " << values.size()
              << ", capacity = " << values.capacity() << "): [";

    for (int i = 0; i < values.size(); ++i)
    {
        std::cout << values[i];

        if (i != values.size() - 1)
            std::cout << ", ";
    }

    std::cout << "]\n";
}

int main()
{
    std::cout << "=== my::vector demo ===\n\n";

    std::cout << "1. Create a vector from an initializer list\n";
    vector<int> numbers = {10, 20, 30};
    printVector(numbers, "Numbers");

    std::cout << "\n2. Change an element using operator[]\n";
    numbers[1] = 99;
    printVector(numbers, "Numbers");

    std::cout << "\n3. Add elements with push_back\n";
    std::cout << "   Capacity grows automatically when the vector is full.\n";
    for (int value = 40; value <= 90; value += 10)
        numbers.push_back(value);
    printVector(numbers, "Numbers");

    std::cout << "\n4. Reserve capacity for future elements\n";
    numbers.reserve(20);
    printVector(numbers, "Numbers");

    std::cout << "\n5. Resize the vector\n";
    numbers.resize(5);
    printVector(numbers, "After resize(5)");
    numbers.resize(8);
    printVector(numbers, "After resize(8)");

    std::cout << "\n6. Copy the vector and modify the copy\n";
    vector<int> copy = numbers;
    copy[0] = -1;
    printVector(numbers, "Original");
    printVector(copy, "Copy");

    std::cout << "\n7. Construct a vector with a given size\n";
    vector<int> zeros(3);
    printVector(zeros, "Zeros");

    std::cout << "\n8. Move-construct a new vector\n";
    vector<int> source = {1, 2, 3};
    vector<int> moved = std::move(source);
    printVector(moved, "Moved-to vector");
    printVector(source, "Moved-from vector");

    std::cout << "\n9. Move-assign into an existing vector\n";
    vector<int> destination = {100, 200};
    destination = std::move(moved);
    printVector(destination, "Destination");
    printVector(moved, "Moved-from vector");

    std::cout << "\n10. Try invalid access\n";
    try
    {
        std::cout << numbers[numbers.size()] << '\n';
    }
    catch (const std::out_of_range& exception)
    {
        std::cout << "   Caught exception: " << exception.what() << '\n';
    }

    std::cout << "\n=== Demo finished ===\n";
    return 0;
}
