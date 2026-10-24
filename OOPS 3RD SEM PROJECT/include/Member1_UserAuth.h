#ifndef MEMBER1_USER_AUTH_H
#define MEMBER1_USER_AUTH_H

#include <string>
#include <vector>
#include <iostream>

// ============================================================================
// PART 1 (DEVELOPED BY TEAM MEMBER 1):
// USER AUTHENTICATION, PROFILE MANAGEMENT & ROLE-BASED ACCESS CONTROL
// Designed for B.Tech IT-B 3rd Semester
//
// OOP Concepts Demonstrated:
// 1. Data Encapsulation & Information Hiding (private attributes with getters/setters)
// 2. Class Inheritance (Base class User -> Derived classes Student & ClassRepresentative)
// 3. Polymorphism & Virtual Functions (dynamic dispatch for role permissions & serialization)
// 4. Abstract Base Class (pure virtual method getRoleName)
// 5. File Persistence & Session Management (loading/saving users)
// ============================================================================

// Base Abstract Class
class User {
protected:
    std::string id;
    std::string username;
    std::string password;
    std::string fullName;
    std::string rollNumber;
    std::string enrollmentNumber;
    int semester;
    std::string branch;
    std::string section;

public:
    // Constructors
    User();
    User(const std::string& id, const std::string& username, const std::string& password,
         const std::string& fullName, const std::string& rollNumber, const std::string& enrollmentNumber,
         int semester = 3, const std::string& branch = "Information Technology", const std::string& section = "IT-B");
    
    // Virtual destructor for safe polymorphic destruction
    virtual ~User() {}

    // Encapsulation: Getters
    std::string getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getFullName() const { return fullName; }
    std::string getRollNumber() const { return rollNumber; }
    std::string getEnrollmentNumber() const { return enrollmentNumber; }
    int getSemester() const { return semester; }
    std::string getBranch() const { return branch; }
    std::string getSection() const { return section; }

    // Encapsulation: Setters / Profile Updaters
    void setFullName(const std::string& name) { fullName = name; }
    void setRollNumber(const std::string& roll) { rollNumber = roll; }
    void setEnrollmentNumber(const std::string& enr) { enrollmentNumber = enr; }
    void setSemester(int sem) { semester = sem; }
    void setBranch(const std::string& b) { branch = b; }
    void setSection(const std::string& sec) { section = sec; }

    bool verifyPassword(const std::string& inputPassword) const {
        return password == inputPassword;
    }

    // OOP CONCEPT: Pure Virtual Function (Making User an Abstract Class)
    virtual std::string getRoleName() const = 0;

    // OOP CONCEPT: Virtual Functions for Polymorphic Permission Checks
    virtual bool canManageAssignments() const { return false; }
    virtual bool canDeclareHolidays() const { return false; }

    // Serialization for C++ File I/O and JSON Web API
    virtual std::string toJson() const;
    virtual std::string toFileString() const;
    virtual void printDetails() const;
};

// Derived Class 1: Regular Student (Classmate)
class Student : public User {
public:
    Student();
    Student(const std::string& id, const std::string& username, const std::string& password,
            const std::string& fullName, const std::string& rollNumber, const std::string& enrollmentNumber,
            int semester = 3, const std::string& branch = "Information Technology", const std::string& section = "IT-B");

    // Overriding pure virtual method
    virtual std::string getRoleName() const;

    virtual bool canManageAssignments() const { return false; }
    virtual bool canDeclareHolidays() const { return false; }

    virtual std::string toJson() const;
    virtual std::string toFileString() const;
    virtual void printDetails() const;
};

// Derived Class 2: Class Representative (CR)
class ClassRepresentative : public User {
private:
    std::string term; // e.g. "2026-2027"

public:
    ClassRepresentative();
    ClassRepresentative(const std::string& id, const std::string& username, const std::string& password,
                        const std::string& fullName, const std::string& rollNumber, const std::string& enrollmentNumber,
                        int semester = 3, const std::string& branch = "Information Technology", const std::string& section = "IT-B",
                        const std::string& term = "2026-2027");

    // Overriding pure virtual method
    virtual std::string getRoleName() const;

    // CR has elevated administrative permissions
    virtual bool canManageAssignments() const { return true; }
    virtual bool canDeclareHolidays() const { return true; }

    std::string getTerm() const { return term; }

    virtual std::string toJson() const;
    virtual std::string toFileString() const;
    virtual void printDetails() const;
};

// Manager class handling all users, authentication, profile updates, and file persistence
class UserManager {
private:
    std::vector<User*> users;
    std::string dataFilePath;

    void clearUsers();

public:
    UserManager(const std::string& filePath = "data/users.txt");
    ~UserManager();

    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

    bool loadFromFile();
    bool saveToFile() const;
    void initializeDefaultUsers();

    User* authenticate(const std::string& username, const std::string& password);
    User* findById(const std::string& id) const;
    User* findByUsername(const std::string& username) const;

    bool updateProfile(const std::string& userId, const std::string& fullName,
                       const std::string& rollNumber, const std::string& enrollmentNumber,
                       int semester, const std::string& branch, const std::string& section);

    bool registerStudent(const std::string& username, const std::string& password,
                         const std::string& fullName, const std::string& rollNumber,
                         const std::string& enrollmentNumber, int semester = 3,
                         const std::string& branch = "Information Technology", const std::string& section = "IT-B");

    const std::vector<User*>& getAllUsers() const { return users; }
    std::string getAllUsersJson() const;
};

#endif // MEMBER1_USER_AUTH_H
