#ifndef QUEUE_H
#define QUEUE_H

// Linked-list queue: first in, first out (FIFO).
template <typename T>
class Queue {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };
    Node* front_;
    Node* rear_;
    int size_;

public:
    Queue() : front_(nullptr), rear_(nullptr), size_(0) {}
    ~Queue() {
        while (front_) {
            Node* t = front_;
            front_ = front_->next;
            delete t;
        }
    }
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    bool isEmpty() const { return front_ == nullptr; }
    int size() const { return size_; }

    // Add to the back: O(1)
    void enqueue(const T& v) {
        Node* n = new Node(v);
        if (rear_) rear_->next = n;
        else front_ = n;
        rear_ = n;
        size_++;
    }

    // Remove from the front: O(1)
    bool dequeue(T& out) {
        if (!front_) return false;
        Node* t = front_;
        out = t->value;
        front_ = t->next;
        if (!front_) rear_ = nullptr;
        delete t;
        size_--;
        return true;
    }

    // Look at the front without removing: O(1)
    bool peek(T& out) const {
        if (!front_) return false;
        out = front_->value;
        return true;
    }

    // Is this value already waiting in the queue? O(n)
    bool contains(const T& v) const {
        for (Node* c = front_; c; c = c->next)
            if (c->value == v) return true;
        return false;
    }
};

#endif
