#ifndef EMPLOYEE_ARRAY_H
#define EMPLOYEE_ARRAY_H

#include "Employee.h"

// Dynamic array that grows when full. Always kept sorted by employee ID
// so that binary search can be used.
class EmployeeArray {
private:
    Employee* data_;
    int size_;
    int capacity_;

    void grow() {
        int newCap = capacity_ * 2;
        Employee* bigger = new Employee[newCap];
        for (int i = 0; i < size_; i++) bigger[i] = data_[i];
        delete[] data_;
        data_ = bigger;
        capacity_ = newCap;
    }

public:
    EmployeeArray() : data_(new Employee[4]), size_(0), capacity_(4) {}
    ~EmployeeArray() { delete[] data_; }
    EmployeeArray(const EmployeeArray&) = delete;
    EmployeeArray& operator=(const EmployeeArray&) = delete;

    int size() const { return size_; }
    Employee& at(int i) { return data_[i]; }
    const Employee& at(int i) const { return data_[i]; }
    const Employee* raw() const { return data_; }

    // Insert in sorted position (shifts bigger IDs to the right): O(n)
    void insertSorted(const Employee& e) {
        if (size_ == capacity_) grow();
        int i = size_ - 1;
        while (i >= 0 && data_[i].id > e.id) {
            data_[i + 1] = data_[i];
            i--;
        }
        data_[i + 1] = e;
        size_++;
    }
};

#endif