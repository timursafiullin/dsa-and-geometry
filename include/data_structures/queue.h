#pragma once

#include <stdexcept>
#include "ListNode.h"

namespace dsa
{

template<typename T>
class queue
{
    ListNode<T>* head;
    ListNode<T>* tail;
public:
    queue() : head(nullptr) { tail = head; }

    ~queue()
    {
        while (head != nullptr)
        {
            ListNode<T>* cur = head;
            head = head->next;
            delete cur;
        }
        tail = nullptr;
    }

    queue(const queue&) = delete;
    queue& operator=(const queue&) = delete;

    T front() const
    { 
        if (head)
            return head->value;
        throw std::out_of_range("Queue is empty.");
    }

    T back() const
    { 
        if (tail)
            return tail->value;
        throw std::out_of_range("Queue is empty.");
    }


    void push(T value)
    {
        if (!tail)
        {
            head = tail = new ListNode<T>(value);
            return;
        }

        ListNode<T>* cur = tail;
        tail = new ListNode<T>(value);
        cur->next = tail;
    }

    void pop()
    {
        if (!head) return;

        ListNode<T>* cur = head;

        if (!head->next)
        {
            tail = nullptr;
            head = nullptr;
        }
        else 
            head = head->next;

        delete cur;
    }
};

}
