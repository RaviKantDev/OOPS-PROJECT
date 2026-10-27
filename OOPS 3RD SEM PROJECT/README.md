# 🌿 CampusFlow - B.Tech IT-B (3rd Sem) Academic & Attendance Portal
> **A 3rd Semester Object-Oriented Programming (OOPS) in C++ Project**  
> *Developed for B.Tech Information Technology (Section IT-B, 3rd Semester)*  
> *Features a Native C++ Object-Oriented Backend and a Calm, Soothing Minimalist Web Interface.*

---

## 🌟 Overview
**CampusFlow** is a complete college portal tailored for **B.Tech IT-B 3rd Semester** students with confidential role-based access control:

1. **Authentication Gate & Privacy:**
   - Dedicated login screen. Students cannot see other classmates' profiles or attendance without logging in.
   - Individual credentials for CR and students.
2. **Profile Management:**
   - Option to edit student details anytime: **Full Name**, **Roll Number**, **Enrollment Number**, **Semester**, and **Section**.
3. **Unified Assignment Hub:**
   - Find all IT-B assignments in one place with details, deadlines, and specifically **where to write** (*Practical Copy*, *Stick File*, *Loose Graph Sheets*, *Spiral File*, *Assignment Copy*).
   - **Real-Time Auto Sync:** When the CR publishes or removes an assignment, it updates automatically on all classmates' screens without refreshing!
4. **Subject-Wise Attendance & Safe Bunk Calculator:**
   - Daily attendance marking (Present / Absent / Class Cancelled).
   - **Custom Subject Additions:** Students can add their own electives, labs, or extra subjects (e.g. *DSA Lab*, *Python Elective*) on working days, and their safe miss predictions update in real time!
   - **Dual Threshold Math:** Calculates how many classes a student can safely bunk while staying **$\ge 75\%$** (standard criterion) and **$\ge 65\%$** (medical condonation limit), or how many consecutive classes they must attend to recover.
5. **Smart Calendar Restrictions:**
   - Upcoming / future dates are strictly locked.
   - **Saturday & Sunday** are permanently fixed non-instructional days.
   - **CR-Declared College Holidays** automatically disable attendance for all classmates.

---

## 🔑 Login Credentials

| Role | Name | Username | Password | Roll Number | Enrollment Number |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Class Representative (CR)** | **Priyanshu Raj (CR)** | `priyanshu_cr` | `priyanshu123` | `25008258` | `GGV/25/234/0064` |
| **Classmate (Student 1)** | **Prakash Kumar** | `prakash` | `prakash123` | `23ITB25` | `0101IT231025` |
| **Classmate (Student 2)** | **Ravikant** | `ravikant` | `ravi123` | `25008264 | `GGV/25/234/0088` |

*(New student accounts can also be registered directly on the portal)*

---

## 👥 3-Team Member Division (For Lab Viva & Defense)

```
                                  CAMPUSFLOW ARCHITECTURE (IT-B)
                                                │
       ┌────────────────────────────────────────┼────────────────────────────────────────┐
       │                                        │                                        │
▼ Member 1: User Auth & Profile         ▼ Member 2: Assignment Hub              ▼ Member 3: Attendance & Math
- Abstract Base Class: User             - Class Design: Assignment              - Custom Exception Hierarchy
- Inheritance: Student & CR             - Operator Overloading (<, <<)          - Zeller's Congruence Date Math
- Polymorphic Permissions               - STL Composition & Sorting             - Dual-Threshold Safe Bunk Math
- Profile Editing & Persistence         - Real-time Assignment Sync             - Dynamic Subject Enroller
([Member1_UserAuth.h](file:///d:/OOPS%203RD%20SEM%20PROJECT/include/Member1_UserAuth.h))    ([Member2_AssignmentHub.h](file:///d:/OOPS%203RD%20SEM%20PROJECT/include/Member2_AssignmentHub.h))    ([Member3_AttendanceTracker.h](file:///d:/OOPS%203RD%20SEM%20PROJECT/include/Member3_AttendanceTracker.h))
```

### Module Breakdown:
1. **Ravikant — Authentication, RBAC & Profile Management:**
   - **Files:** [`include/Member1_UserAuth.h`](file:///d:/OOPS%203RD%20SEM%20PROJECT/include/Member1_UserAuth.h) & [`src/Member1_UserAuth.cpp`](file:///d:/OOPS%203RD%20SEM%20PROJECT/src/Member1_UserAuth.cpp)
   - **Concepts:** Abstract Base Class (`User`), Pure Virtual Function (`getRoleName()`), Inheritance (`Student`, `ClassRepresentative`), Polymorphic Permission Dispatch, Profile editing (roll no, enrollment no, semester), and File Persistence (`data/users.txt`).

2. **Prakash Kumar — Assignment Management Hub:**
   - **Files:** [`include/Member2_AssignmentHub.h`](file:///d:/OOPS%203RD%20SEM%20PROJECT/include/Member2_AssignmentHub.h) & [`src/Member2_AssignmentHub.cpp`](file:///d:/OOPS%203RD%20SEM%20PROJECT/src/Member2_AssignmentHub.cpp)
   - **Concepts:** Class Encapsulation, Operator Overloading (`operator<` for sorting by deadline, `operator<<` for output stream), Composition (`AssignmentManager`), Submission Medium Filters, and File Persistence (`data/assignments.txt`).

3. **Priyanshu Raj — Attendance Engine, Dynamic Subjects & Bunk Math:**
   - **Files:** [`include/Member3_AttendanceTracker.h`](file:///d:/OOPS%203RD%20SEM%20PROJECT/include/Member3_AttendanceTracker.h), [`src/Member3_AttendanceTracker.cpp`](file:///d:/OOPS%203RD%20SEM%20PROJECT/src/Member3_AttendanceTracker.cpp), and [`include/CommonUtils.h`](file:///d:/OOPS%203RD%20SEM%20PROJECT/include/CommonUtils.h)
   - **Concepts:** Custom Exception Hierarchy (`FutureDateException`, `WeekendException`, `HolidayException`), Calendar Math (Zeller's Congruence), Dynamic Subject Addition (`data/student_subjects.txt`), Predictive Safe Miss & Recovery Formulas, and File Persistence (`data/attendance.txt`, `data/holidays.txt`).

---

## 🚀 How to Run

### Option 1: Live Web Portal Mode
Double-click:
```cmd
run_server.bat
```
* Compiles the C++ web server and launches it on `http://localhost:8080`.
* Opens your browser automatically!

### Option 2: Interactive Terminal CLI Mode 
Double-click:
```cmd
run_cli.bat
```
* Runs the terminal menu demonstrating login, profile updates, assignment sorting, and exception handling directly in the console.

---

## 🧮 Mathematical Safe Miss (Bunk) Formula

Given:
- $P$ = Classes attended (Present)
- $T$ = Total classes held
- $X$ = Target attendance percentage ($75\%$ or $65\%$)

### 1. When current attendance $\ge X\%$:
$$\text{Safe Misses} = \left\lfloor \frac{100P - XT}{X} \right\rfloor$$

### 2. When current attendance $< X\%$:
$$\text{Consecutive Classes to Attend} = \left\lceil \frac{XT - 100P}{100 - X} \right\rceil$$

---

## 📄 Viva Preparation
Refer to [`Viva_Preparation_Guide.md`](file:///d:/OOPS%203RD%20SEM%20PROJECT/Viva_Preparation_Guide.md) for detailed viva questions, model answers, and concept explanations for each member.
