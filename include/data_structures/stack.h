#pragma once

#include <stdexcept>
#include "ListNode.h"

namespace dsa
{

template<typename T>
class stack
{
    ListNode<T>* head;
public:
    stack() : head(nullptr) {}

    ~stack()
    {
        while (head != nullptr)
        {
            ListNode<T>* cur = head;
            head = head->next;
            delete cur;
        }
    }

    stack(const stack&) = delete;
    stack& operator=(const stack&) = delete;

    T top() const
    {
        if (head)
            return head->value;
        throw std::out_of_range("Stack is empty.");
    }

    void push(T value)
    {
        head = new ListNode<T>(value, head);
    }

    void pop()
    {
        if (!head) return;

        ListNode<T>* cur = head;
        head = head->next;
        delete cur;
    }
};

}
