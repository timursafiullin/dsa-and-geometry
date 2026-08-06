#include "../include/data_structures/doubly_linked_list.h"

#include <iostream>
#include <stdexcept>

using namespace dsa;

void printList(doubly_linked_list<int>& list, int size, const char* label)
{
    std::cout << label << ": [";
    for (int i = 0; i < size; ++i)
    {
        std::cout << list.get(i);
        if (i != size - 1)
            std::cout << ", ";
    }
    std::cout << "]\n";
}

int main()
{
    doubly_linked_list<int> list;
    int size = 0;

    std::cout << "=== dsa::doubly_linked_list demo ===\n\n";

    std::cout << "1. Add elements at head and tail\n";
    list.add_at_head(2);
    ++size;
    list.add_at_head(1);
    ++size;
    list.add_at_tail(4);
    ++size;
    printList(list, size, "List");

    std::cout << "\n2. Add 3 at index 2\n";
    list.add_at_index(2, 3);
    ++size;
    printList(list, size, "List");

    std::cout << "\n3. Delete element at index 1\n";
    list.delete_at_index(1);
    --size;
    printList(list, size, "List");

    std::cout << "\n4. Add 5 at the tail\n";
    list.add_at_tail(5);
    ++size;
    printList(list, size, "List");

    std::cout << "\n5. Try invalid deletion\n";
    try
    {
        list.delete_at_index(size);
    }
    catch (const std::out_of_range& exception)
    {
        std::cout << "   Caught exception: " << exception.what() << '\n';
    }

    std::cout << "\n=== Demo finished ===\n";
    return 0;
}
