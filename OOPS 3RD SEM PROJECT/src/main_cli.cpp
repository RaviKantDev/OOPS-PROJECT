#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "../include/CommonUtils.h"
#include "../include/Member1_UserAuth.h"
#include "../include/Member2_AssignmentHub.h"
#include "../include/Member3_AttendanceTracker.h"

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"

void printHeader() {
    std::cout << "\n" << CYAN << BOLD;
    std::cout << "======================================================================\n";
    std::cout << "   CAMPUSFLOW: B.TECH IT-B 3RD SEMESTER ACADEMIC & ATTENDANCE PORTAL  \n";
    std::cout << "======================================================================\n" << RESET;
    std::cout << "System Date : " << CommonUtils::getTodayDate() 
              << " (" << CommonUtils::getDayName(CommonUtils::getTodayDate()) << ")\n";
    std::cout << "Branch/Sec  : Information Technology (IT-B) | Semester: 3\n";
    std::cout << "======================================================================\n";
}

int main() {
    UserManager userManager("data/users.txt");
    AssignmentManager assignmentManager("data/assignments.txt");
    AttendanceTracker attendanceTracker("data/attendance.txt", "data/holidays.txt", "data/student_subjects.txt");

    User* currentUser = NULL;

    while (true) {
        printHeader();

        if (currentUser == NULL) {
            std::cout << YELLOW << "Status: Not logged in. Please sign in to access your profile.\n" << RESET;
            std::cout << "\n" << BOLD << "--- [AUTHENTICATION & PROFILES] ---\n" << RESET;
            std::cout << " 1. Login as CR (Priyanshu Raj) -> priyanshu_cr / priyanshu123\n";
            std::cout << " 2. Login as Student (Prakash Kumar) -> prakash / prakash123\n";
            std::cout << " 3. Login as Student (Ravikant) -> ravikant / ravi123\n";
            std::cout << " 4. Custom Login (Username & Password)\n";
            std::cout << " 5. Register New Student Account\n";
            std::cout << "\n" << BOLD << "--- [PUBLIC ACCESS] ---\n" << RESET;
            std::cout << " 6. View All Assignments (Member 2 Module)\n";
            std::cout << " 7. View College Holidays (Member 3 Module)\n";
            std::cout << " 0. Exit System\n";
            std::cout << "\nEnter choice: ";

            int choice = -1;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::string dummy;
                std::getline(std::cin, dummy);
                continue;
            }
            std::cin.ignore();

            if (choice == 0) {
                std::cout << "\nExiting Campus Portal. Goodbye!\n";
                break;
            } else if (choice == 1) {
                currentUser = userManager.authenticate("priyanshu_cr", "priyanshu123");
                if (currentUser) std::cout << GREEN << "\nLogged in as CR (Priyanshu Raj)!\n" << RESET;
            } else if (choice == 2) {
                currentUser = userManager.authenticate("prakash", "prakash123");
                if (currentUser) std::cout << GREEN << "\nLogged in as Student (Prakash Kumar)!\n" << RESET;
            } else if (choice == 3) {
                currentUser = userManager.authenticate("ravikant", "ravi123");
                if (currentUser) std::cout << GREEN << "\nLogged in as Student (Ravikant)!\n" << RESET;
            } else if (choice == 4) {
                std::string u, p;
                std::cout << "Enter username: ";
                std::getline(std::cin, u);
                std::cout << "Enter password: ";
                std::getline(std::cin, p);
                currentUser = userManager.authenticate(u, p);
                if (currentUser) {
                    std::cout << GREEN << "\nWelcome, " << currentUser->getFullName() << "!\n" << RESET;
                } else {
                    std::cout << RED << "\nInvalid username or password!\n" << RESET;
                }
            } else if (choice == 5) {
                std::string u, p, name, roll, enr;
                std::cout << "Enter new username: ";
                std::getline(std::cin, u);
                std::cout << "Enter password: ";
                std::getline(std::cin, p);
                std::cout << "Enter full name: ";
                std::getline(std::cin, name);
                std::cout << "Enter roll number: ";
                std::getline(std::cin, roll);
                std::cout << "Enter enrollment number: ";
                std::getline(std::cin, enr);

                if (userManager.registerStudent(u, p, name, roll, enr, 3, "Information Technology", "IT-B")) {
                    std::cout << GREEN << "\nRegistration successful! You can now log in.\n" << RESET;
                } else {
                    std::cout << RED << "\nUsername already exists!\n" << RESET;
                }
            } else if (choice == 6) {
                std::cout << "\n" << BOLD << "--- ASSIGNMENTS LIST (Sorted by Deadline) ---\n" << RESET;
                std::vector<Assignment> list = assignmentManager.getSortedAssignments();
                for (size_t i = 0; i < list.size(); ++i) {
                    std::cout << list[i];
                }
            } else if (choice == 7) {
                std::cout << "\n" << BOLD << "--- COLLEGE HOLIDAYS LIST ---\n" << RESET;
                const std::vector<CollegeHoliday>& hols = attendanceTracker.getHolidayManager().getAllHolidays();
                for (size_t i = 0; i < hols.size(); ++i) {
                    std::cout << "Date: " << hols[i].getDate() 
                              << " | Reason: " << hols[i].getDescription()
                              << " | Declared By: " << hols[i].getDeclaredBy() << "\n";
                }
            }
        } else {
            // USER IS AUTHENTICATED
            std::cout << GREEN << BOLD << "Logged In As: " << currentUser->getFullName() 
                      << " [" << currentUser->getRoleName() << "] - Roll: " << currentUser->getRollNumber()
                      << " | Enr: " << currentUser->getEnrollmentNumber() << "\n" << RESET;

            std::cout << "\n" << BOLD << "--- [MEMBER 1: PROFILE & AUTH] ---\n" << RESET;
            std::cout << " 1. View My Profile (Polymorphic printDetails())\n";
            std::cout << " 2. Edit Profile (Roll No, Enrollment No, Semester)\n";
            std::cout << " 3. Logout\n";

            std::cout << "\n" << BOLD << "--- [MEMBER 2: ASSIGNMENT HUB] ---\n" << RESET;
            std::cout << " 4. View All Assignments (Sorted by Deadline)\n";
            std::cout << " 5. Filter Assignments by Submission Medium (Copy / Stick File)\n";
            if (currentUser->canManageAssignments()) {
                std::cout << " 6. " << YELLOW << "[CR ONLY] Add New Assignment\n" << RESET;
                std::cout << " 7. " << YELLOW << "[CR ONLY] Delete Assignment\n" << RESET;
            }

            std::cout << "\n" << BOLD << "--- [MEMBER 3: ATTENDANCE & SUBJECT ENGINE] ---\n" << RESET;
            std::cout << " 8. Mark Daily Attendance (With Exception Handling Demo)\n";
            std::cout << " 9. View My Subject Attendance & Safe Miss (Bunk) Calculator\n";
            std::cout << " 10. Add Custom Subject to My Curriculum\n";
            std::cout << " 11. View Declared College Holidays\n";
            if (currentUser->canDeclareHolidays()) {
                std::cout << " 12. " << YELLOW << "[CR ONLY] Declare New College Holiday\n" << RESET;
                std::cout << " 13. " << YELLOW << "[CR ONLY] Remove College Holiday\n" << RESET;
            }
            std::cout << " 0. Exit System\n";
            std::cout << "\nEnter choice: ";

            int choice = -1;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::string dummy;
                std::getline(std::cin, dummy);
                continue;
            }
            std::cin.ignore();

            if (choice == 0) {
                std::cout << "\nExiting Campus Portal. Goodbye!\n";
                break;
            } else if (choice == 1) {
                std::cout << "\n" << BOLD << "=== POLYMORPHIC USER DETAILS ===\n" << RESET;
                currentUser->printDetails();
            } else if (choice == 2) {
                std::string name, roll, enr, sec;
                int sem = 3;
                std::cout << "Enter Full Name [" << currentUser->getFullName() << "]: ";
                std::getline(std::cin, name);
                if (name.empty()) name = currentUser->getFullName();
                std::cout << "Enter Roll Number [" << currentUser->getRollNumber() << "]: ";
                std::getline(std::cin, roll);
                if (roll.empty()) roll = currentUser->getRollNumber();
                std::cout << "Enter Enrollment Number [" << currentUser->getEnrollmentNumber() << "]: ";
                std::getline(std::cin, enr);
                if (enr.empty()) enr = currentUser->getEnrollmentNumber();
                std::cout << "Enter Semester [" << currentUser->getSemester() << "]: ";
                std::cin >> sem;
                std::cin.ignore();

                if (userManager.updateProfile(currentUser->getId(), name, roll, enr, sem, currentUser->getBranch(), currentUser->getSection())) {
                    std::cout << GREEN << "\nProfile updated successfully!\n" << RESET;
                } else {
                    std::cout << RED << "\nFailed to update profile!\n" << RESET;
                }
            } else if (choice == 3) {
                currentUser = NULL;
                std::cout << YELLOW << "\nLogged out successfully.\n" << RESET;
            } else if (choice == 4) {
                std::vector<Assignment> list = assignmentManager.getSortedAssignments();
                for (size_t i = 0; i < list.size(); ++i) {
                    std::cout << list[i];
                }
            } else if (choice == 5) {
                std::cout << "\nSelect Medium to Filter:\n";
                std::cout << "1. Practical Copy\n2. Stick File\n3. Loose Graph Sheets\n4. Assignment Copy\n5. Spiral File\n";
                std::cout << "Choice: ";
                int mChoice = 1;
                std::cin >> mChoice;
                std::cin.ignore();
                std::string targetMedium = "Practical Copy";
                if (mChoice == 2) targetMedium = "Stick File";
                else if (mChoice == 3) targetMedium = "Loose Graph Sheets";
                else if (mChoice == 4) targetMedium = "Assignment Copy";
                else if (mChoice == 5) targetMedium = "Spiral File";

                std::vector<Assignment> list = assignmentManager.getAssignmentsByMedium(targetMedium);
                std::cout << "\nFound " << list.size() << " assignments written on: " << targetMedium << "\n";
                for (size_t i = 0; i < list.size(); ++i) {
                    std::cout << list[i];
                }
            } else if (choice == 6 && currentUser->canManageAssignments()) {
                std::string code, subject, title, desc, assigned, deadline, medium, faculty;
                int marks = 20;
                std::cout << "Enter Subject Code (e.g. IT301): ";
                std::getline(std::cin, code);
                std::cout << "Enter Subject Name: ";
                std::getline(std::cin, subject);
                std::cout << "Enter Assignment Title: ";
                std::getline(std::cin, title);
                std::cout << "Enter Description: ";
                std::getline(std::cin, desc);
                std::cout << "Enter Assigned Date (YYYY-MM-DD): ";
                std::getline(std::cin, assigned);
                if (assigned.empty()) assigned = CommonUtils::getTodayDate();
                std::cout << "Enter Deadline Date (YYYY-MM-DD): ";
                std::getline(std::cin, deadline);
                std::cout << "Enter Submission Medium (Stick File, Practical Copy, etc.): ";
                std::getline(std::cin, medium);
                std::cout << "Enter Faculty Name: ";
                std::getline(std::cin, faculty);

                Assignment asgn("", code, subject, title, desc, assigned, deadline, medium, faculty, marks);
                assignmentManager.addAssignment(asgn);
                std::cout << GREEN << "\nAssignment created successfully!\n" << RESET;
            } else if (choice == 7 && currentUser->canManageAssignments()) {
                std::string id;
                std::cout << "Enter Assignment ID to delete: ";
                std::getline(std::cin, id);
                if (assignmentManager.deleteAssignment(id)) {
                    std::cout << GREEN << "Assignment deleted.\n" << RESET;
                } else {
                    std::cout << RED << "Assignment not found!\n" << RESET;
                }
            } else if (choice == 8) {
                std::vector<std::string> mySubjects = attendanceTracker.getSubjectsForStudent(currentUser->getId());
                std::cout << "\n" << BOLD << "=== MARK DAILY ATTENDANCE (MEMBER 3) ===\n" << RESET;
                std::cout << "Select Subject:\n";
                for (size_t i = 0; i < mySubjects.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". " << mySubjects[i] << "\n";
                }
                std::cout << "Choice: ";
                int subChoice = 1;
                std::cin >> subChoice;
                std::cin.ignore();
                if (subChoice < 1 || subChoice > (int)mySubjects.size()) subChoice = 1;
                std::string chosenSubject = mySubjects[subChoice - 1];

                std::string dateStr;
                std::cout << "Enter Date (YYYY-MM-DD) [Today: " << CommonUtils::getTodayDate() << "]: ";
                std::getline(std::cin, dateStr);
                if (dateStr.empty()) dateStr = CommonUtils::getTodayDate();

                std::cout << "Enter Status (1 = PRESENT, 2 = ABSENT, 3 = CANCELLED): ";
                int stChoice = 1;
                std::cin >> stChoice;
                std::cin.ignore();
                std::string status = "PRESENT";
                if (stChoice == 2) status = "ABSENT";
                else if (stChoice == 3) status = "CANCELLED";

                try {
                    attendanceTracker.markAttendance(currentUser->getId(), chosenSubject, dateStr, status);
                    std::cout << GREEN << "\nSuccess: Attendance marked as " << status 
                              << " for " << chosenSubject << " on " << dateStr << "!\n" << RESET;
                } catch (const FutureDateException& e) {
                    std::cout << RED << "\n[EXCEPTION CAUGHT]: " << e.what() << "\n"
                              << "-> Rule: Attendance cannot be logged for future upcoming dates.\n" << RESET;
                } catch (const WeekendException& e) {
                    std::cout << RED << "\n[EXCEPTION CAUGHT]: " << e.what() << "\n"
                              << "-> Rule: Saturday and Sunday are fixed non-instructional days.\n" << RESET;
                } catch (const HolidayException& e) {
                    std::cout << RED << "\n[EXCEPTION CAUGHT]: " << e.what() << "\n"
                              << "-> Rule: No classes held on official CR-declared college holidays.\n" << RESET;
                } catch (const std::exception& e) {
                    std::cout << RED << "\n[GENERAL EXCEPTION]: " << e.what() << "\n" << RESET;
                }
            } else if (choice == 9) {
                std::cout << "\n" << BOLD << "======================================================================\n";
                std::cout << "          SUBJECT-WISE ATTENDANCE & SAFE MISS (BUNK) CALCULATOR       \n";
                std::cout << "======================================================================\n" << RESET;
                std::vector<SubjectAttendanceStats> stats = attendanceTracker.getAllSubjectStats(currentUser->getId());

                for (size_t i = 0; i < stats.size(); ++i) {
                    const SubjectAttendanceStats& s = stats[i];
                    std::cout << BOLD << "\n[" << (i + 1) << "] " << s.subjectName << RESET << "\n";
                    std::cout << "    Attended: " << s.attendedCount << " / " << s.totalHeld 
                              << " | Cancelled: " << s.cancelledCount << "\n";
                    std::cout << "    Percentage: " << std::fixed << std::setprecision(1) << s.percentage << "% ";

                    if (s.percentage >= 75.0) {
                        std::cout << GREEN << "[SAFE] -> Can safely bunk " << s.safeMisses75 << " class(es)!\n" << RESET;
                    } else {
                        std::cout << RED << "[SHORTAGE] -> Must attend " << s.requiredAttend75 << " consecutive class(es)!\n" << RESET;
                    }

                    if (s.percentage >= 65.0) {
                        std::cout << CYAN << "    Medical Limit (65%): Can bunk " << s.safeMisses65 << " class(es)!\n" << RESET;
                    } else {
                        std::cout << RED << "    Medical Limit (65%): Critical! Attend " << s.requiredAttend65 << " class(es)!\n" << RESET;
                    }
                }
                std::cout << "\n======================================================================\n";
            } else if (choice == 10) {
                std::string newSubject;
                std::cout << "Enter New Subject / Lab / Elective Name: ";
                std::getline(std::cin, newSubject);
                if (attendanceTracker.addCustomSubject(currentUser->getId(), newSubject)) {
                    std::cout << GREEN << "Subject '" << newSubject << "' added successfully to your curriculum!\n" << RESET;
                } else {
                    std::cout << RED << "Subject already exists or invalid name.\n" << RESET;
                }
            } else if (choice == 11) {
                const std::vector<CollegeHoliday>& hols = attendanceTracker.getHolidayManager().getAllHolidays();
                for (size_t i = 0; i < hols.size(); ++i) {
                    std::cout << "  * " << hols[i].getDate() << " : " << hols[i].getDescription()
                              << " (Added by: " << hols[i].getDeclaredBy() << ")\n";
                }
            } else if (choice == 12 && currentUser->canDeclareHolidays()) {
                std::string dateStr, reason;
                std::cout << "Enter Holiday Date (YYYY-MM-DD): ";
                std::getline(std::cin, dateStr);
                std::cout << "Enter Reason: ";
                std::getline(std::cin, reason);
                if (attendanceTracker.getHolidayManager().addHoliday(dateStr, reason, currentUser->getFullName())) {
                    std::cout << GREEN << "Holiday declared.\n" << RESET;
                } else {
                    std::cout << RED << "Failed to add holiday.\n" << RESET;
                }
            } else if (choice == 13 && currentUser->canDeclareHolidays()) {
                std::string dateStr;
                std::cout << "Enter Holiday Date to remove (YYYY-MM-DD): ";
                std::getline(std::cin, dateStr);
                if (attendanceTracker.getHolidayManager().removeHoliday(dateStr)) {
                    std::cout << GREEN << "Holiday removed.\n" << RESET;
                } else {
                    std::cout << RED << "Holiday not found.\n" << RESET;
                }
            }
        }

        std::cout << "\nPress Enter to continue...";
        std::string dummy;
        std::getline(std::cin, dummy);
    }

    return 0;
}
