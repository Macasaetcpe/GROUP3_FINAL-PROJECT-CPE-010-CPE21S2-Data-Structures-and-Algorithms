#ifndef ATTENDANCE_SYSTEM_H
#define ATTENDANCE_SYSTEM_H

#include <iomanip>
#include <iostream>
#include <string>

#include "Algorithms.h"
#include "AttendanceLog.h"
#include "Employee.h"
#include "EmployeeArray.h"
#include "Queue.h"
#include "Stack.h"
#include "Utils.h"

// A late time-in waiting for the team lead to review.
struct LateEntry {
    int empId = 0;
    int timeIn = 0;
};

class AttendanceSystem {
private:
    EmployeeArray employees_;              // sorted by ID (binary search)
    AttendanceLog log_;                    // every time-in / time-out (doubly linked list)
    Queue<LateEntry> lateQueue_;           // late arrivals, first late = first reviewed
    Stack<AttendanceRecord*> undoStack_;   // newest time-ins first (undo)

    static const int DEPT_COUNT = 5;
    static const char* departmentName(int i) {
        static const char* names[DEPT_COUNT] = {"Customer Service", "Technical Support", "Sales",
                                                "Back Office", "HR"};
        return names[i];
    }

    // ---------- small helpers ----------
    int findIndex(int id) const {
        return binarySearchById(employees_.raw(), employees_.size(), id);
    }

    // Asks for an ID and finds the employee. Returns index, or -1 if not found.
    int askForEmployee() {
        int id = Utils::readInt("Employee ID: ", 1, 999999);
        int idx = findIndex(id);
        if (idx < 0) std::cout << "  [Error] Employee ID " << id << " is not registered.\n";
        return idx;
    }

    static std::string hoursText(int minutes) {
        return std::to_string(minutes / 60) + "h " + std::to_string(minutes % 60) + "m";
    }

    static std::string statusText(const AttendanceRecord& r) {
        std::string s = r.late ? "Late" : "On time";
        if (r.timeOut < 0) s += ", still in";
        else if (r.earlyOut) s += ", left early";
        return s;
    }

public:
    // ---------- 1. Register ----------
    void registerEmployee() {
        std::cout << "\n--- Register Employee ---\n";
        Employee e;
        e.id = Utils::readInt("Employee ID (1-999999): ", 1, 999999);
        if (findIndex(e.id) >= 0) {
            std::cout << "  [Error] ID " << e.id << " already exists.\n";
            return;
        }
        e.name = Utils::readNonEmpty("Full name: ");
        for (int i = 0; i < DEPT_COUNT; i++) std::cout << "  " << i + 1 << ". " << departmentName(i) << "\n";
        e.department = departmentName(Utils::readInt("Department (1-5): ", 1, DEPT_COUNT) - 1);
        employees_.insertSorted(e);
        std::cout << "  Registered " << e.name << " (" << e.department << ").\n";
    }

    // ---------- 2. Time in ----------
    void timeIn() {
        std::cout << "\n--- Time In ---\n";
        int idx = askForEmployee();
        if (idx < 0) return;
        const Employee& e = employees_.at(idx);
        if (e.status != ACTIVE) {
            std::cout << "  [Error] " << e.name << " is on " << statusToString(e.status)
                      << " and cannot time in.\n";
            return;
        }
        if (log_.findOpenEntry(e.id)) {
            std::cout << "  [Error] " << e.name << " already timed in and has not timed out.\n";
            return;
        }
        int t = Utils::readTimeOrNow("Time in (HH:MM, or press Enter for the current time): ");

        AttendanceRecord r;
        r.empId = e.id;
        r.name = e.name;
        r.department = e.department;
        r.timeIn = t;
        r.late = (t > SHIFT_START);
        undoStack_.push(log_.append(r));

        std::cout << "  " << e.name << " timed in at " << Utils::formatTime(t);
        if (r.late) {
            lateQueue_.enqueue(LateEntry{e.id, t});
            std::cout << " - LATE by " << (t - SHIFT_START) << " min";
        } else {
            std::cout << " - on time";
        }
        std::cout << " (shift starts " << Utils::formatTime(SHIFT_START) << ").\n";
    }

    // ---------- 3. Time out ----------
    void timeOut() {
        std::cout << "\n--- Time Out ---\n";
        int idx = askForEmployee();
        if (idx < 0) return;
        const Employee& e = employees_.at(idx);
        AttendanceRecord* rec = log_.findOpenEntry(e.id);
        if (!rec) {
            std::cout << "  [Error] " << e.name << " has not timed in.\n";
            return;
        }
        int t = Utils::readTimeOrNow("Time out (HH:MM, or press Enter for the current time): ");
        if (t < rec->timeIn) {
            std::cout << "  [Error] Time out " << Utils::formatTime(t) << " is earlier than time in "
                      << Utils::formatTime(rec->timeIn) << ". Nothing was saved.\n";
            return;
        }
        rec->timeOut = t;
        rec->earlyOut = (t < SHIFT_END);
        std::cout << "  " << e.name << " timed out at " << Utils::formatTime(t) << ". Worked "
                  << hoursText(t - rec->timeIn) << (rec->earlyOut ? " (left early)" : "") << ".\n";
    }

    // ---------- 4. Search ----------
    void searchEmployee() {
        std::cout << "\n--- Search Employee ---\n";
        int idx = askForEmployee();
        if (idx < 0) return;
        const Employee& e = employees_.at(idx);
        std::cout << "  ID:         " << e.id << "\n"
                  << "  Name:       " << e.name << "\n"
                  << "  Department: " << e.department << "\n"
                  << "  Status:     " << statusToString(e.status) << "\n";

        int id = e.id;
        auto mine = [id](const AttendanceRecord& r) { return r.empId == id; };
        int count = 0;
        for (AttendanceRecord* c = linearSearchList(log_.head(), mine); c != nullptr;
             c = linearSearchList(c->next, mine)) {
            count++;
            std::cout << "  Record " << count << ": in " << Utils::formatTime(c->timeIn) << ", out "
                      << Utils::formatTime(c->timeOut) << " (" << statusText(*c) << ")\n";
        }
        if (count == 0) std::cout << "  No attendance records yet.\n";
    }

    // ---------- 5. Report (merge sort) ----------
    void showReport() {
        std::cout << "\n--- Attendance Report ---\n";
        if (log_.size() == 0) {
            std::cout << "  No attendance records yet.\n";
            return;
        }
        std::cout << "  Sort by: 1. Employee ID  2. Name  3. Department  4. Time in\n";
        int key = Utils::readInt("Choose (1-4): ", 1, 4);

        int n = log_.size();
        AttendanceRecord* rows = log_.toArray();
        if (key == 1) {
            mergeSort(rows, n, [](const AttendanceRecord& a, const AttendanceRecord& b) {
                return a.empId < b.empId;
            });
        } else if (key == 2) {
            mergeSort(rows, n, [](const AttendanceRecord& a, const AttendanceRecord& b) {
                return Utils::toLower(a.name) < Utils::toLower(b.name);
            });
        } else if (key == 3) {
            mergeSort(rows, n, [](const AttendanceRecord& a, const AttendanceRecord& b) {
                if (a.department != b.department) return a.department < b.department;
                return Utils::toLower(a.name) < Utils::toLower(b.name);
            });
        } else {
            mergeSort(rows, n, [](const AttendanceRecord& a, const AttendanceRecord& b) {
                if (a.timeIn != b.timeIn) return a.timeIn < b.timeIn;
                return a.empId < b.empId;
            });
        }

        std::cout << "\n" << std::left << std::setw(7) << "ID" << std::setw(20) << "Name"
                  << std::setw(19) << "Department" << std::setw(8) << "In" << std::setw(8) << "Out"
                  << std::setw(9) << "Hours" << "Status\n"
                  << std::string(82, '-') << "\n";
        for (int i = 0; i < n; i++) {
            const AttendanceRecord& r = rows[i];
            std::string hours = (r.timeOut >= 0) ? hoursText(r.timeOut - r.timeIn) : "--";
            std::cout << std::left << std::setw(7) << r.empId << std::setw(20) << r.name.substr(0, 19)
                      << std::setw(19) << r.department << std::setw(8) << Utils::formatTime(r.timeIn)
                      << std::setw(8) << Utils::formatTime(r.timeOut) << std::setw(9) << hours
                      << statusText(r) << "\n";
        }
        delete[] rows;
    }

    // ---------- 6. Review late arrivals (queue) ----------
    void reviewLateArrival() {
        std::cout << "\n--- Review Late Arrivals ---\n";
        LateEntry le;
        while (lateQueue_.dequeue(le)) {
            // Skip entries whose time-in was undone
            AttendanceRecord* rec = linearSearchList(log_.head(), [&le](const AttendanceRecord& r) {
                return r.empId == le.empId && r.timeIn == le.timeIn;
            });
            if (!rec) continue;
            std::cout << "  Next late arrival: " << rec->name << " (" << rec->department << ")\n"
                      << "  Timed in at " << Utils::formatTime(rec->timeIn) << ", "
                      << (rec->timeIn - SHIFT_START) << " min after the shift start.\n"
                      << "  Marked as reviewed.\n";
            return;
        }
        std::cout << "  No late arrivals waiting for review.\n";
    }

    // ---------- 7. Undo ----------
    void undoLastTimeIn() {
        std::cout << "\n--- Undo Last Time In ---\n";
        AttendanceRecord* last;
        if (!undoStack_.pop(last)) {
            std::cout << "  Nothing to undo.\n";
            return;
        }
        std::cout << "  Removed the time in of " << last->name << " at "
                  << Utils::formatTime(last->timeIn) << ".\n";
        log_.remove(last);
    }

    // ---------- 8. Status ----------
    void updateStatus() {
        std::cout << "\n--- Update Employee Status ---\n";
        int idx = askForEmployee();
        if (idx < 0) return;
        Employee& e = employees_.at(idx);
        std::cout << "  " << e.name << " is currently: " << statusToString(e.status) << "\n"
                  << "  1. Active  2. Sick Leave  3. Paid Leave  4. Maternity Leave\n";
        e.status = (Status)Utils::readInt("New status (1-4): ", 1, 4);
        std::cout << "  Status updated to " << statusToString(e.status) << ".\n";
    }

    // ---------- Demo data (only with: ./attendtrack --demo) ----------
    void loadDemoData() {
        struct Row { int id; const char* name; int dept; };
        Row rows[] = {{1005, "Juan Dela Cruz", 0}, {1001, "Maria Santos", 1}, {1010, "Ana Reyes", 2},
                      {1003, "Pedro Garcia", 0},   {1008, "Carla Lim", 3},    {1002, "Rico Bautista", 1}};
        for (const Row& r : rows) {
            Employee e;
            e.id = r.id;
            e.name = r.name;
            e.department = departmentName(r.dept);
            employees_.insertSorted(e);
        }
        std::cout << "Demo mode: loaded " << employees_.size() << " sample employees.\n";
    }
};

#endif
