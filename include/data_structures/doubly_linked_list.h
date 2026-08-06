#pragma once

#include <stdexcept>
#include "ListNode.h"

namespace dsa
{

template<typename T>
class doubly_linked_list
{
public:
    doubly_linked_list() : head(nullptr), tail(nullptr) {}

    ~doubly_linked_list()
    {
        while (head != nullptr)
        {
            ListNode<T>* cur = head;
            head = head->next;
            delete cur;
        }
        tail = nullptr;
    }

    T get(int index)
    {
        if (index < 0)
            throw std::invalid_argument("Index cannot be negative.");

        ListNode<T>* cur = head;
        for (int i = 0; i < index && cur != nullptr; ++i)
            cur = cur->next;

        if (cur == nullptr)
            throw std::out_of_range("Index out of range.");

        return cur->value;
    }

    void add_at_head(T val)
    {
        ListNode<T>* newNode = new ListNode<T>(val);

        if (head == nullptr)
        {
            head = tail = newNode;
            return;
        }

        newNode->next = head;
        head->pred = newNode;
        head = newNode;
    }

    void add_at_tail(T val)
    {
        ListNode<T>* newNode = new ListNode<T>(val);

        if (tail == nullptr)
        {
            head = tail = newNode;
            return;
        }

        newNode->pred = tail;
        tail->next = newNode;
        tail = newNode;
    }

    void add_at_index(int index, T val)
    {
        if (index < 0)
            throw std::invalid_argument("Index cannot be negative.");
        if (index == 0)
        {
            add_at_head(val);
            return;
        }

        ListNode<T>* cur = head;
        for (int i = 0; i < index - 1 && cur != nullptr; ++i)
            cur = cur->next;

        if (cur == nullptr)
            throw std::out_of_range("Index out of range.");

        ListNode<T>* newNode = new ListNode<T>(val, cur->next);
        newNode->pred = cur;

        if (cur->next != nullptr)
            cur->next->pred = newNode;
        else
            tail = newNode;

        cur->next = newNode;
    }

    void delete_at_index(int index)
    {
        if (index < 0)
            throw std::invalid_argument("Index cannot be negative.");

        ListNode<T>* cur = head;
        for (int i = 0; i < index && cur != nullptr; ++i)
            cur = cur->next;

        if (cur == nullptr)
            throw std::out_of_range("Index out of range.");

        if (cur->pred != nullptr)
            cur->pred->next = cur->next;
        else
            head = cur->next;

        if (cur->next != nullptr)
            cur->next->pred = cur->pred;
        else
            tail = cur->pred;

        delete cur;
    }

private:
    ListNode<T>* head;
    ListNode<T>* tail;
};

}
