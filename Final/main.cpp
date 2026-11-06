#include <iostream>
#include <string>
#include "AttendanceSystem.h"
#include "Utils.h"

void showMenu() {
    std::cout << "\n=========== ATTENDTRACK ===========\n"
              << " 1. Register employee\n"
              << " 2. Time in\n"
              << " 3. Time out\n"
              << " 4. Search employee\n"
              << " 5. Attendance report\n"
              << " 6. Review late arrivals\n"
              << " 7. Undo last time in\n"
              << " 8. Update employee status\n"
              << " 0. Exit\n"
              << "===================================\n";
}

int main(int argc, char* argv[]) {
    AttendanceSystem system;
    std::cout << "AttendTrack - Employee Attendance Tracking\n";
    if (argc > 1 && std::string(argv[1]) == "--demo") system.loadDemoData();

    while (true) {
        showMenu();
        int choice = 0;
        if (!Utils::tryParseInt(Utils::readLine("Enter your choice: "), choice) || choice < 0 ||
            choice > 8) {
            std::cout << "  [Invalid] Please choose a number from the menu (0-8).\n";
            continue;
        }
        switch (choice) {
            case 1: system.registerEmployee(); break;
            case 2: system.timeIn(); break;
            case 3: system.timeOut(); break;
            case 4: system.searchEmployee(); break;
            case 5: system.showReport(); break;
            case 6: system.reviewLateArrival(); break;
            case 7: system.undoLastTimeIn(); break;
            case 8: system.updateStatus(); break;
            case 0: std::cout << "Goodbye!\n"; return 0;
        }
    }
}
