#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

enum Status { ACTIVE = 1, SICK_LEAVE = 2, PAID_LEAVE = 3, MATERNITY_LEAVE = 4 };

inline std::string statusToString(Status s) {
    switch (s) {
        case ACTIVE: return "Active";
        case SICK_LEAVE: return "Sick Leave";
        case PAID_LEAVE: return "Paid Leave";
        case MATERNITY_LEAVE: return "Maternity Leave";
    }
    return "Unknown";
}

struct Employee {
    int id = 0;
    std::string name;
    std::string department;
    Status status = ACTIVE;
};

#endif
