#ifndef STACK_H
#define STACK_H

// Linked-list stack: last in, first out (LIFO).
template <typename T>
class Stack {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v, Node* n) : value(v), next(n) {}
    };
    Node* top_;
    int size_;

public:
    Stack() : top_(nullptr), size_(0) {}
    ~Stack() {
        while (top_) {
            Node* t = top_;
            top_ = top_->next;
            delete t;
        }
    }
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    bool isEmpty() const { return top_ == nullptr; }
    int size() const { return size_; }

    // O(1)
    void push(const T& v) {
        top_ = new Node(v, top_);
        size_++;
    }

    // O(1)
    bool pop(T& out) {
        if (!top_) return false;
        Node* t = top_;
        out = t->value;
        top_ = t->next;
        delete t;
        size_--;
        return true;
    }

    // O(1)
    bool peek(T& out) const {
        if (!top_) return false;
        out = top_->value;
        return true;
    }
};

#endif