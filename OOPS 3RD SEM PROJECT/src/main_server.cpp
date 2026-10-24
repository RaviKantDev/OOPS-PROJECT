#include <iostream>
#include "../include/HttpServer.h"

int main() {
    int port = 8080;

    std::cout << "Initializing College Portal C++ Backend...\n";
    std::cout << "[Member 1 Module]: Initializing User Authentication & Roles...\n";
    UserManager userManager("data/users.txt");

    std::cout << "[Member 2 Module]: Initializing Assignment Management Hub...\n";
    AssignmentManager assignmentManager("data/assignments.txt");

    std::cout << "[Member 3 Module]: Initializing Attendance Tracker & Holiday Engine...\n";
    AttendanceTracker attendanceTracker("data/attendance.txt", "data/holidays.txt");

    CollegeWebServer server(port, userManager, assignmentManager, attendanceTracker);
    server.start();

    return 0;
}

