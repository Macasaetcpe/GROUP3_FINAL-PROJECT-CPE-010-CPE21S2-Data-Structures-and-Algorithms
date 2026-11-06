#ifndef ATTENDANCE_LOG_H
#define ATTENDANCE_LOG_H

#include <string>

#include "Algorithms.h"

struct AttendanceRecord {
    int empId = 0;
    std::string name;
    std::string department;
    int timeIn = -1;    // minutes after midnight, -1 = not set
    int timeOut = -1;   // -1 = still working (open entry)
    bool late = false;
    bool earlyOut = false;
    AttendanceRecord* prev = nullptr;
    AttendanceRecord* next = nullptr;
};

// Doubly linked list of attendance records (newest at the tail).
// Doubly linked so that removing any node (for Undo) is O(1).
class AttendanceLog {
private:
    AttendanceRecord* head_;
    AttendanceRecord* tail_;
    int count_;

public:
    AttendanceLog() : head_(nullptr), tail_(nullptr), count_(0) {}
    ~AttendanceLog() {
        while (head_) {
            AttendanceRecord* t = head_;
            head_ = head_->next;
            delete t;
        }
    }
    AttendanceLog(const AttendanceLog&) = delete;
    AttendanceLog& operator=(const AttendanceLog&) = delete;

    int size() const { return count_; }
    AttendanceRecord* head() const { return head_; }

    // Add to the end: O(1). Returns pointer to the new node.
    AttendanceRecord* append(const AttendanceRecord& r) {
        AttendanceRecord* n = new AttendanceRecord(r);
        n->prev = tail_;
        n->next = nullptr;
        if (tail_) tail_->next = n;
        else head_ = n;
        tail_ = n;
        count_++;
        return n;
    }

    // Remove a known node: O(1)
    void remove(AttendanceRecord* n) {
        if (n->prev) n->prev->next = n->next;
        else head_ = n->next;
        if (n->next) n->next->prev = n->prev;
        else tail_ = n->prev;
        delete n;
        count_--;
    }

    // Linear search for an employee's open entry (timed in, not yet out): O(n)
    AttendanceRecord* findOpenEntry(int empId) const {
        return linearSearchList(head_, [empId](const AttendanceRecord& r) {
            return r.empId == empId && r.timeOut < 0;
        });
    }

    // Copy the log into a new array (caller must delete[]): O(n)
    AttendanceRecord* toArray() const {
        AttendanceRecord* arr = new AttendanceRecord[count_ > 0 ? count_ : 1];
        int i = 0;
        for (AttendanceRecord* c = head_; c; c = c->next) {
            arr[i] = *c;
            arr[i].prev = nullptr;
            arr[i].next = nullptr;
            i++;
        }
        return arr;
    }
};

#endif