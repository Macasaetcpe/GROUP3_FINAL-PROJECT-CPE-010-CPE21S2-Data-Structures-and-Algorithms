#include <iostream>
#include <string>
#include <vector>
 
#include "Queue.h"
 
using namespace std;
 
struct Employee {
    int id;
    string name;
    string department;
};

int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) exit(0);  
        try {
            size_t used;
            int v = stoi(line, &used);
            if (used == line.size()) return v;
        } catch (...) {}
        cout << "  [Invalid] Please type a whole number.\n";
    }
}
 

int findEmployee(const vector<Employee>& list, int id) {
    for (size_t i = 0; i < list.size(); i++)
        if (list[i].id == id) return (int)i;
    return -1;
}
 
void registerEmployee(vector<Employee>& list) {
    Employee e;
    e.id = readInt("Employee ID: ");
    if (findEmployee(list, e.id) != -1) {
        cout << "  [Error] That ID already exists.\n";
        return;
    }
    cout << "Full name: ";
    getline(cin, e.name);
    cout << "Department: ";
    getline(cin, e.department);
    list.push_back(e);
    cout << "  Registered " << e.name << ".\n";
}
 
void addToQueue(const vector<Employee>& list, Queue<int>& q) {
    int id = readInt("Employee ID to add to the queue: ");
    int idx = findEmployee(list, id);
    if (idx == -1) {
        cout << "  [Error] Employee not found.\n";
        return;
    }
    if (q.contains(id)) {
        cout << "  [Error] Already in the queue.\n";
        return;
    }
    q.enqueue(id);
    cout << "  " << list[idx].name << " added. Position in queue: " << q.size() << "\n";
}
 
void processNext(const vector<Employee>& list, Queue<int>& q) {
    int id;
    if (!q.dequeue(id)) {
        cout << "  The queue is empty.\n";
        return;
    }
    int idx = findEmployee(list, id);
    cout << "  Now serving: " << list[idx].name << " (" << list[idx].department << ")\n";
    int hour = readInt("Time-in hour (0-23): ");
    int minute = readInt("Time-in minute (0-59): ");
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        cout << "  [Error] Invalid time. Please try again later.\n";
        return;
    }
    bool late = (hour * 60 + minute) > 8 * 60;  // 8 Start ng shift any time past that is yung considered late
    cout << "  Time-in recorded: " << hour << ":" << (minute < 10 ? "0" : "") << minute << " - "
         << (late ? "LATE" : "ON TIME") << "\n";
}
 
int main() {
    vector<Employee> employees;
    Queue<int> checkInQueue;
    int choice;
    do {
        cout << "\n===== ATTENDTRACK (Di pa to final overview lang) =====\n"
             << "1. Register employee\n2. Add to time-in queue\n3. Process next check-in\n"
             << "0. Exit\n";
        choice = readInt("Choice: ");
        if (choice == 1) registerEmployee(employees);
        else if (choice == 2) addToQueue(employees, checkInQueue);
        else if (choice == 3) processNext(employees, checkInQueue);
        else if (choice != 0) cout << "  [Invalid] Choose 0-3.\n";
    } while (choice != 0);
    cout << "Goodbye!\n";
    return 0;
}