# GROUP3_FINAL-PROJECT-CPE-010-CPE21S2-Data-Structures-and-Algorithms

# AttendTrack: Employee Attendance Tracking System

**CPE 010 Data Structures and Algorithms | CPE21S2 | Group 3**
**Instructor:** Engr. Jimlord M. Quejado

AttendTrack is a C++ console program that replaces the paper logbook used to track employee attendance in a BPO company (modeled on Teleperformance). Employees time in and time out by ID, and the program records late arrivals, early outs, hours worked, and leave status.

## Features

| Menu | Feature | What it does |
|---|---|---|
| 1 | Register employee | Saves ID, name, and department (duplicate IDs are rejected) |
| 2 | Time in | Records the time in and marks it on time or late (shift starts 08:00) |
| 3 | Time out | Records the time out, hours worked, and early out (shift ends 17:00) |
| 4 | Search employee | Shows an employee's details, status, and attendance records |
| 5 | Attendance report | Sorted by ID, name, department, or time in |
| 6 | Review late arrivals | Team lead reviews late employees, first late first reviewed |
| 7 | Undo last time in | Removes the most recent time in |
| 8 | Update employee status | Active, Sick Leave, Paid Leave, Maternity Leave |
| 0 | Exit | Ends the program |

## Data Structures and Algorithms Used

| Topic | Used for |
|---|---|
| Queue (linked list) | Late arrivals waiting for review |
| Stack (linked list) | Undo last time in |
| Doubly linked list | Attendance log |
| Dynamic array | Employee records, kept sorted by ID |
| Binary search | Finding an employee by ID |
| Linear search | Finding an open record and employee history |
| Merge sort | Sorting the attendance report |

## Project Files

| File | Purpose |
|---|---|
| `main.cpp` | Menu and program loop |
| `AttendanceSystem.h` | All the features (register, time in, time out, search, report, review, undo, status) |
| `Utils.h` | Input checking and time helpers |
| `Employee.h` | Employee record and status |
| `EmployeeArray.h` | Sorted dynamic array |
| `Queue.h` | Linked-list queue |
| `Stack.h` | Linked-list stack |
| `AttendanceLog.h` | Doubly linked list of attendance records |
| `Algorithms.h` | Binary search, linear search, merge sort |
| `benchmark.cpp` | Timing code for the performance analysis |

## How to Compile and Run

Requires a C++17 compiler (g++ 9 or newer).

```
g++ -std=c++17 -O2 main.cpp -o attendtrack
./attendtrack            # Windows: attendtrack.exe
./attendtrack --demo     # starts with 6 sample employees for testing
```

When the program asks for a time, type it in 24-hour format (for example `08:15`) or press Enter to use the current time.

## Sample Input and Output

Start with `./attendtrack --demo`, then enter: `2` `1005` `08:05`, then `2` `1001` `07:50`, then `3` `1005` `17:00`, then `6`, then `5` `1`.

```
--- Time In ---
  Juan Dela Cruz timed in at 08:05 - LATE by 5 min (shift starts 08:00).
--- Time In ---
  Maria Santos timed in at 07:50 - on time (shift starts 08:00).
--- Time Out ---
  Juan Dela Cruz timed out at 17:00. Worked 8h 55m.
--- Review Late Arrivals ---
  Next late arrival: Juan Dela Cruz (Customer Service)
  Timed in at 08:05, 5 min after the shift start.
  Marked as reviewed.
--- Attendance Report ---
ID     Name                Department         In      Out     Hours    Status
----------------------------------------------------------------------------------
1001   Maria Santos        Technical Support  07:50   --:--   --       On time, still in
1005   Juan Dela Cruz      Customer Service   08:05   17:00   8h 55m   Late
```

## Performance Benchmark

```
g++ -std=c++17 -O2 benchmark.cpp -o benchmark
./benchmark
```

It times binary search, linear search, merge sort, and queue enqueue for 100 to 100,000 items.

## Limitations

- Data is kept in memory only and is lost when the program closes.
- One working day at a time (no calendar dates), one fixed shift (08:00 to 17:00), single user.

## Team Members

| Name | Role |
|---|---|
| Barizo, Julian Benedict D.R|  |
| Cruz, Elijah J. | Leon Scott Kennedy |
| Esteta, Jesse |  |
| Fernandez, Cedric S. |  |
| Macasaet, John David C. |  |
