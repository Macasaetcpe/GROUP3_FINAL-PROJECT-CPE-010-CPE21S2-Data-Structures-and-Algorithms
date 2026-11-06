#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

// Employee info and status types
// Employee statuses (numbers match the menu choices)
enum Status { ACTIVE = 1, SICK_LEAVE = 2, PAID_LEAVE = 3, MATERNITY_LEAVE = 4 };

// Turns a status into text for printing
inline std::string statusToString(Status s) {
    switch (s) {
        case ACTIVE: return "Active";
        case SICK_LEAVE: return "Sick Leave";
        case PAID_LEAVE: return "Paid Leave";
        case MATERNITY_LEAVE: return "Maternity Leave";
    }
    return "Unknown";
}

// Stores one employee's details
// EmployeeArray keeps these sorted by id so binarySearchById() works.
struct Employee {
    int id = 0;                // unique employee ID (1-999999), the search key
    std::string name;          // full name
    std::string department;    // one of the departments listed in main.cpp
    Status status = ACTIVE;    // current status; new employees start as ACTIVE
};

#endif
