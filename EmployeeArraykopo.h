#ifndef EMPLOYEE_ARRAY_H
#define EMPLOYEE_ARRAY_H

#include "Employee.h"

class EmployeeArray {
private:
    Employee* data_;
    int size_;
    int capacity_;

    void grow() {
    }

public:
    EmployeeArray() {
    }
    ~EmployeeArray() {
    }
    EmployeeArray(const EmployeeArray&) = delete;
    EmployeeArray& operator=(const EmployeeArray&) = delete;

    int size() const {
    }
    Employee& at(int i) {
    }
    const Employee& at(int i) const {
    }
    const Employee* raw() const {
    }

    void insertSorted(const Employee& e) {
    }
};

#endif
