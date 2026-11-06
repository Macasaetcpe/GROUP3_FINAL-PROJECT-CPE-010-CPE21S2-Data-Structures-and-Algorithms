#ifndef STACK_H
#define STACK_H

template <typename T>
class Stack {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v, Node* n) {
        }
    };
    Node* top_;
    int size_;

public:
    Stack() {
    }
    ~Stack() {
    }
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    bool isEmpty() const {
    }
    int size() const {
    }

    void push(const T& v) {
    }

    bool pop(T& out) {
    }

    bool peek(T& out) const {
    }
};

#endif
