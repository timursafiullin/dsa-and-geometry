#pragma once

#include <stdexcept>
#include "ListNode.h"

namespace dsa
{

template<typename T>
class singly_linked_list
{
public:
    singly_linked_list() : head(nullptr), tail(nullptr) {}

    ~singly_linked_list()
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
        newNode->next = head;
        head = newNode;

        if (tail == nullptr)
            tail = newNode;
    }

    void add_at_tail(T val)
    {
        ListNode<T>* newNode = new ListNode<T>(val);

        if (tail == nullptr)
        {
            head = tail = newNode;
            return;
        }

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
        cur->next = newNode;

        if (newNode->next == nullptr)
            tail = newNode;
    }

    void delete_at_index(int index)
    {
        if (index < 0)
            throw std::invalid_argument("Index cannot be negative.");
        if (head == nullptr)
            throw std::out_of_range("Index out of range.");

        if (index == 0)
        {
            ListNode<T>* oldNode = head;
            head = head->next;
            delete oldNode;

            if (head == nullptr)
                tail = nullptr;
            return;
        }

        ListNode<T>* cur = head;
        for (int i = 0; i < index - 1 && cur != nullptr; ++i)
            cur = cur->next;

        if (cur == nullptr || cur->next == nullptr)
            throw std::out_of_range("Index out of range.");

        ListNode<T>* oldNode = cur->next;
        cur->next = oldNode->next;

        if (oldNode == tail)
            tail = cur;

        delete oldNode;
    }

private:
    ListNode<T>* head;
    ListNode<T>* tail;
};

}
