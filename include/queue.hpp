#pragma once

#include <memory_resource>
#include <vector>
#include <concepts>
#include <memory>
#include <exception>
#include <algorithm>


template <class T, class AllocatorType>
    requires std::is_default_constructible_v<T> && std::is_same_v<AllocatorType, std::pmr::polymorphic_allocator<T>>

class Queue {
private:
    struct qNode {
        T data;
        qNode *next;

        qNode() : data(), next(nullptr) {}
        qNode(const T &value) : data(value), next(nullptr) {}
        qNode(T &&value) : data(std::move(value)), next(nullptr) {}
        ~qNode() = default;
    };

    using qNodeAllocator = typename std::allocator_traits<AllocatorType>::template rebind_alloc<qNode>;
    
    qNodeAllocator allocator;
    qNode *head = nullptr;
    qNode *tail = nullptr;
    size_t size = 0;

public:
    explicit Queue(const AllocatorType &alloc = {}) : allocator(alloc) {}

    void push(const T &value) {
        qNode *new_node = allocator.allocate(1);
        allocator.construct(new_node, value);

        if (!tail) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        size++;
    }

    void push(T &&value) {
        qNode *new_node = allocator.allocate(1);
        allocator.construct(new_node, std::move(value));

        if (!tail) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        size++;
    }

    void pop() {
        if (!head) { return; }

        qNode *tmp = head;
        head = head->next;

        if (!head) { tail = nullptr; }

        allocator.destroy(tmp);
        allocator.deallocate(tmp, 1);
        size--;
    }

    size_t get_size() const { return size; }

    bool is_empty() const { return size == 0; }

    T& front() { return head->data; }
    const T& front() const { return head->data; }

    T& back() { return tail->data; }
    const T& back() const { return tail->data; }
    
    void clear() {
        while (!is_empty()) { pop(); }
    }

    ~Queue() {
        clear();
    }

    class Iterator {
    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator() : current(nullptr) {}

        explicit Iterator(qNode *node) : current(node) {}

        reference operator*() const{ return current->data; }
        pointer operator->() const { return &current->data; }

        Iterator &operator++() {
            if (current) { current = current->next; }
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator &other) const { return current == other.current; }
        bool operator!=(const Iterator &other) const { return !(*this == other); }

    private:

        qNode *current;
    
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(tail); }
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }

};
