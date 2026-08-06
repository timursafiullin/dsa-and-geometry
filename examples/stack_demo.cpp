#include "../include/data_structures/stack.h"

#include <iostream>
#include <stdexcept>

using namespace dsa;

int main()
{
    stack<int> values;

    std::cout << "=== dsa::stack demo ===\n\n";

    std::cout << "1. Push elements into the stack\n";
    values.push(10);
    values.push(20);
    values.push(30);
    std::cout << "   Top: " << values.top() << '\n';

    std::cout << "\n2. Pop elements from the stack\n";
    while (true)
    {
        try
        {
            std::cout << "   Popped: " << values.top() << '\n';
            values.pop();
        }
        catch (const std::out_of_range& exception)
        {
            std::cout << "   Stack is empty: " << exception.what() << '\n';
            break;
        }
    }

    std::cout << "\n3. Try to read from an empty stack\n";
    try
    {
        values.top();
    }
    catch (const std::out_of_range& exception)
    {
        std::cout << "   Caught exception: " << exception.what() << '\n';
    }

    std::cout << "\n=== Demo finished ===\n";
    return 0;
}
