# 🎓 OOPS in C++ (3rd Semester) - Comprehensive Viva & Defense Guide
**Project Title:** CampusFlow - College Academic Assignment Hub & Smart Attendance Portal  
**Team Size:** 3 Members  
**Language:** C++ (Standard C++11 / C++14)  
**Backend Architecture:** Object-Oriented C++ Engine + Native Winsock Micro-Server  
**Frontend:** Calm, Soothing Minimalist Web Portal (HTML5, CSS3, Vanilla JS)

---

## 👥 3-Member Module Division & Ownership

| Member | Module Title | Source Files | Primary OOP Concepts Used |
| :--- | :--- | :--- | :--- |
| **Member 1** | **User Authentication & Role-Based Access Control (RBAC)** | `include/Member1_UserAuth.h`<br>`src/Member1_UserAuth.cpp` | • Abstract Base Class (`User`)<br>• Inheritance (`Student`, `ClassRepresentative`)<br>• Pure Virtual Functions (`getRoleName()`)<br>• Polymorphism / Dynamic Binding<br>• Data Encapsulation & File I/O (`users.txt`) |
| **Member 2** | **Assignment Management Hub & Submission Mediums** | `include/Member2_AssignmentHub.h`<br>`src/Member2_AssignmentHub.cpp` | • Class Design & Composition<br>• Operator Overloading (`operator<`, `operator<<`, `operator==`)<br>• Encapsulation (Getters/Setters)<br>• Sorting & Filtering Algorithms<br>• File Persistence (`assignments.txt`) |
| **Member 3** | **Smart Attendance Engine & Bunk / Recovery Calculator** | `include/Member3_AttendanceTracker.h`<br>`src/Member3_AttendanceTracker.cpp`<br>`include/CommonUtils.h` | • Custom Exception Hierarchy (`try-catch-throw`)<br>• Mathematical Modeling & Predictive Logic<br>• Date Algorithm (Zeller's Congruence)<br>• Business Rules Validation (Future Dates & Holidays)<br>• File Persistence (`attendance.txt`, `holidays.txt`) |

---

## 📌 Module 1: Detailed Viva Guide (Team Member 1)

### Role & What You Built
- You built the **User Identity and Security Architecture**.
- You created the abstract base class `User` and two derived classes:
  1. `Student` (represents regular classmates)
  2. `ClassRepresentative` (represents the CR who can add assignments and declare holidays)
- You implemented the `UserManager` class which handles file storage, login verification, and registration.

### Core OOP Concepts & Code References
1. **Abstract Base Class & Pure Virtual Function:**
   ```cpp
   class User {
   public:
       virtual std::string getRoleName() const = 0; // Pure virtual function
   };
   ```
   *Viva Question:* "Why did you make `User` an abstract class?"  
   *Answer:* "Because a generic 'User' does not exist in real college life; a person is either a `Student` or a `ClassRepresentative`. Making `getRoleName()` pure virtual prevents accidental instantiation of `User` and forces derived classes to implement their specific role behavior."

2. **Polymorphic Permission Dispatch:**
   ```cpp
   virtual bool canManageAssignments() const { return false; } // Student returns false
   // In ClassRepresentative:
   virtual bool canManageAssignments() const { return true; }  // CR returns true
   ```
   *Viva Question:* "How does polymorphism prevent unauthorized access?"  
   *Answer:* "We use base class pointers (`User*`). At runtime, C++ uses the Virtual Method Table (vtable) to dynamically invoke the correct version of `canManageAssignments()` depending on whether the object is a `Student` or `ClassRepresentative`. This eliminates messy `if-else` type checks."

3. **Data Encapsulation:**
   - Private members: `username`, `password`, `rollNumber`, `email`.
   - Accessible only through public getters (`getUsername()`, `getRollNumber()`) and verification functions (`verifyPassword()`).

---

## 📌 Module 2: Detailed Viva Guide (Team Member 2)

### Role & What You Built
- You built the **Assignment Management System**.
- You handled the key real-world requirement: *Where do students have to write each assignment?* (e.g., **Practical Copy**, **Stick File**, **Loose Graph Sheets**, **Spiral File**).
- You implemented the `Assignment` entity and the `AssignmentManager` container.

### Core OOP Concepts & Code References
1. **Operator Overloading (`operator<` and `operator<<`):**
   ```cpp
   // Overloading '<' to automatically sort assignments by deadline date
   bool operator<(const Assignment& other) const {
       return this->deadlineDate < other.deadlineDate;
   }

   // Overloading '<<' for direct terminal stream printing
   friend std::ostream& operator<<(std::ostream& os, const Assignment& a) {
       os << "[" << a.subjectCode << "] " << a.title 
          << " | Medium: " << a.submissionMedium << " | Due: " << a.deadlineDate;
       return os;
   }
   ```
   *Viva Question:* "Why did you overload `operator<`?"  
   *Answer:* "Overloading `operator<` enables standard STL algorithms like `std::sort()` to compare two `Assignment` objects natively by their `deadlineDate` without writing external comparison functions."

2. **Object Composition:**
   - `AssignmentManager` has a `std::vector<Assignment>` member, demonstrating the "has-a" relationship (Composition/Aggregation).

3. **File Persistence:**
   - Serializes assignments using a pipe-delimited format (`ID|SubjectCode|SubjectName|Title|Medium|...`) so changes made by the CR persist between runs.

---

## 📌 Module 3: Detailed Viva Guide (Team Member 3)

### Role & What You Built
- You built the **Smart Daily Attendance Tracker & Holiday Management Engine**.
- You implemented strict date validation rules:
  1. Students **cannot** mark attendance on upcoming / future dates.
  2. **Saturday and Sunday** are permanently fixed non-instructional holidays.
  3. **CR-declared holidays** (e.g. Diwali Fest, Sports Day) lock attendance for all students.
- You derived the **Bunk / Safe Miss Mathematical Formula** to calculate classes students can safely miss while keeping attendance $\ge 75\%$ and $\ge 65\%$.

### Core OOP Concepts & Code References
1. **Custom Exception Handling Hierarchy:**
   ```cpp
   class AppException : public std::exception { ... };
   class FutureDateException : public AppException { ... };
   class WeekendException : public AppException { ... };
   class HolidayException : public AppException { ... };
   ```
   *Viva Question:* "Why use custom exceptions instead of returning error codes?"  
   *Answer:* "Exceptions separate error handling from regular business logic. If a student tries to mark attendance on a future date or Sunday, the validation method throws a specific exception like `FutureDateException`. The caller catches it cleanly and presents an explanatory message."

2. **Date Math (Zeller's Congruence):**
   ```cpp
   int getDayOfWeek(int year, int month, int day) {
       if (month < 3) { month += 12; year -= 1; }
       int k = year % 100, j = year / 100;
       int h = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
       return (h + 6) % 7; // 0 = Sunday, 6 = Saturday
   }
   ```
   *Viva Question:* "How do you detect Saturdays and Sundays without hardcoding?"  
   *Answer:* "We implemented Zeller's Congruence, a classic discrete mathematical algorithm that calculates the day of the week for any Gregorian calendar date."

3. **Safe Miss & Recovery Mathematical Derivation:**
   Let:
   - $P$ = Classes attended (Present)
   - $T$ = Total classes held ($P \le T$)
   - $X$ = Target attendance percentage ($75\%$ or $65\%$)

   **Case 1: When current attendance $\ge X\%$:**
   How many additional classes $M$ can be safely missed without dropping below $X\%$?
   $$\frac{P}{T + M} \ge \frac{X}{100}$$
   $$100P \ge X(T + M) = XT + XM$$
   $$XM \le 100P - XT \implies M \le \frac{100P - XT}{X}$$
   $$\mathbf{Safe\ Misses} = \left\lfloor \frac{100P - XT}{X} \right\rfloor$$

   **Case 2: When current attendance $< X\%$:**
   How many consecutive classes $A$ must the student attend to recover to $X\%$?
   $$\frac{P + A}{T + A} \ge \frac{X}{100}$$
   $$100(P + A) \ge X(T + A)$$
   $$100P + 100A \ge XT + XA$$
   $$(100 - X)A \ge XT - 100P \implies A \ge \frac{XT - 100P}{100 - X}$$
   $$\mathbf{Required\ Classes} = \left\lceil \frac{XT - 100P}{100 - X} \right\rceil$$

---

## ⚡ Quick Viva Checklist

| Question | Short Answer for Professor |
| :--- | :--- |
| **Is C++ actually used for the backend?** | Yes, the entire core engine (classes, inheritance, sorting, attendance math, socket server) is written in C++ using MinGW g++. |
| **How does the website talk to C++?** | The C++ backend implements an embedded Winsock HTTP server on port 8080 that serves static web files and responds to REST API requests in JSON format. |
| **Can we run it in terminal without a browser?** | Yes! Run `run_cli.bat` to launch the interactive C++ console demo. |
| **Where is data stored?** | In formatted text files in the `data/` folder (`users.txt`, `assignments.txt`, `attendance.txt`, `holidays.txt`). |

