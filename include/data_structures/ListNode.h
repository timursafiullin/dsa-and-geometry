#pragma once

namespace dsa
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
    
}