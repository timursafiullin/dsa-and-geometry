#include "../include/data_structures/queue.h"

#include <iostream>
#include <stdexcept>

using namespace dsa;

int main()
{
    queue<int> values;

    std::cout << "=== dsa::queue demo ===\n\n";

    std::cout << "1. Push elements into the queue\n";
    values.push(10);
    values.push(20);
    values.push(30);
    std::cout << "   Front: " << values.front() << '\n';
    std::cout << "   Back: " << values.back() << '\n';

    std::cout << "\n2. Pop elements from the queue\n";
    while (true)
    {
        try
        {
            std::cout << "   Popped: " << values.front() << '\n';
            values.pop();
        }
        catch (const std::out_of_range& exception)
        {
            std::cout << "   Queue is empty: " << exception.what() << '\n';
            break;
        }
    }

    std::cout << "\n3. Try to read from an empty queue\n";
    try
    {
        values.front();
    }
    catch (const std::out_of_range& exception)
    {
        std::cout << "   Caught exception: " << exception.what() << '\n';
    }

    std::cout << "\n=== Demo finished ===\n";
    return 0;
}
