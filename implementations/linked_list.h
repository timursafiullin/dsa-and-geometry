#pragma once

#include <initializer_list>
#include <stdexcept>

namespace my
{

template<typename T>
struct ListNode
{
    ListNode(T val) : value(val), pred(nullptr), next(nullptr) {}
    ListNode(T val, ListNode<T>* pointer)
        : value(val), pred(nullptr), next(pointer) {}
    ListNode(T val, ListNode<T>* previous, ListNode<T>* pointer)
        : value(val), pred(previous), next(pointer) {}

    T value;
    ListNode<T>* pred;
    ListNode<T>* next;
};


template<typename T>
class SinglyLinkedList {
public:
    SinglyLinkedList() : head(nullptr) { tail = head; }
    
    T get(int index) {
        if (index < 0)
            throw std::invalid_argument("Index cannot be negative.");

        ListNode<T>* cur = head;
        for (int i = 0; i < index && cur != nullptr; ++i)
            cur = cur->next;

        if (cur == nullptr)
            throw std::out_of_range("Index out of range.");

        return cur->value;
    }
    
    void addAtHead(T val) {
        ListNode<T>* newNode = new ListNode<T>(val);
        if (head == nullptr)
        {
            head = tail = newNode;
            return;
        }
        newNode->next = head;
        head = newNode;
    }
    
    void addAtTail(T val) {
        ListNode<T>* newNode = new ListNode<T>(val);
        if (tail == nullptr)
        {
            head = tail = newNode;
            return;
        }
        tail->next = newNode;
        tail = newNode;
    }
    
    void addAtIndex(int index, T val) {
        if (index < 0)
            throw std::invalid_argument("Index cannot be negative.");

        if (index == 0) 
        {
            addAtHead(val);
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
    
    void deleteAtIndex(int index) {
        if (index < 0)
            throw std::invalid_argument("Index cannot be negative.");
    
        if (index == 0)
        {
            if (head == nullptr)
                throw std::out_of_range("Index out of range.");
            ListNode<T>* cur = head;
            head = head->next;
            if (head == nullptr)
                tail = nullptr;
            delete cur;
            return;
        }

        ListNode<T>* cur = head;
        for (int i = 0; i < index - 1 && cur != nullptr; ++i)
            cur = cur->next;

        if (cur == nullptr || cur->next == nullptr)
            throw std::out_of_range("Index out of range.");

        ListNode<T>* oldNode = cur->next;

        if (oldNode == tail)
            tail = cur;

        cur->next = cur->next->next;
        delete oldNode;
    }
private:
    ListNode<T>* head;
    ListNode<T>* tail;
};

template<typename T>
class DoublyLinkedList {
public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    T get(int index) {
        if (index < 0)
            throw std::invalid_argument("Index cannot be negative.");

        ListNode<T>* cur = head;
        for (int i = 0; i < index && cur != nullptr; ++i)
            cur = cur->next;

        if (cur == nullptr)
            throw std::out_of_range("Index out of range.");

        return cur->value;
    }

    void addAtHead(T val) {
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

    void addAtTail(T val) {
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

    void addAtIndex(int index, T val) {
        if (index < 0)
            throw std::invalid_argument("Index cannot be negative.");
        if (index == 0)
        {
            addAtHead(val);
            return;
        }

        ListNode<T>* cur = head;
        for (int i = 0; i < index - 1 && cur != nullptr; ++i)
            cur = cur->next;

        if (cur == nullptr)
            throw std::out_of_range("Index out of range.");

        ListNode<T>* newNode = new ListNode<T>(val, cur, cur->next);
        if (cur->next != nullptr)
            cur->next->pred = newNode;
        else
            tail = newNode;
        cur->next = newNode;
    }

    void deleteAtIndex(int index) {
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
