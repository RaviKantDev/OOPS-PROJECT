#include "../include/Member1_UserAuth.h"
#include "../include/CommonUtils.h"
#include <fstream>
#include <sstream>
#include <iomanip>

// ============================================================================
// Base User Implementation
// ============================================================================
User::User()
    : id(""), username(""), password(""), fullName(""),
      rollNumber(""), enrollmentNumber(""), semester(3),
      branch("Information Technology"), section("IT-B") {}

User::User(const std::string& id, const std::string& username, const std::string& password,
           const std::string& fullName, const std::string& rollNumber, const std::string& enrollmentNumber,
           int semester, const std::string& branch, const std::string& section)
    : id(id), username(username), password(password), fullName(fullName),
      rollNumber(rollNumber), enrollmentNumber(enrollmentNumber), semester(semester),
      branch(branch), section(section) {}

std::string User::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << "\"id\":\"" << CommonUtils::escapeJson(id) << "\","
       << "\"username\":\"" << CommonUtils::escapeJson(username) << "\","
       << "\"fullName\":\"" << CommonUtils::escapeJson(fullName) << "\","
       << "\"rollNumber\":\"" << CommonUtils::escapeJson(rollNumber) << "\","
       << "\"enrollmentNumber\":\"" << CommonUtils::escapeJson(enrollmentNumber) << "\","
       << "\"semester\":" << semester << ","
       << "\"branch\":\"" << CommonUtils::escapeJson(branch) << "\","
       << "\"section\":\"" << CommonUtils::escapeJson(section) << "\","
       << "\"role\":\"" << getRoleName() << "\","
       << "\"canManageAssignments\":" << (canManageAssignments() ? "true" : "false") << ","
       << "\"canDeclareHolidays\":" << (canDeclareHolidays() ? "true" : "false")
       << "}";
    return ss.str();
}

std::string User::toFileString() const {
    // Pipe-delimited storage format:
    // ROLE|ID|USERNAME|PASSWORD|FULLNAME|ROLLNUMBER|ENROLLMENT|SEMESTER|BRANCH|SECTION
    std::ostringstream ss;
    ss << getRoleName() << "|"
       << id << "|"
       << username << "|"
       << password << "|"
       << fullName << "|"
       << rollNumber << "|"
       << enrollmentNumber << "|"
       << semester << "|"
       << branch << "|"
       << section;
    return ss.str();
}

void User::printDetails() const {
    std::cout << "User ID    : " << id << "\n"
              << "Name       : " << fullName << "\n"
              << "Username   : " << username << "\n"
              << "Roll No    : " << rollNumber << "\n"
              << "Enrollment : " << enrollmentNumber << "\n"
              << "Course/Sec : " << branch << " (Section " << section << "), Sem " << semester << "\n"
              << "Role       : " << getRoleName() << "\n";
}

// ============================================================================
// Student Implementation (Derived Class 1)
// ============================================================================
Student::Student() : User() {}

Student::Student(const std::string& id, const std::string& username, const std::string& password,
                 const std::string& fullName, const std::string& rollNumber, const std::string& enrollmentNumber,
                 int semester, const std::string& branch, const std::string& section)
    : User(id, username, password, fullName, rollNumber, enrollmentNumber, semester, branch, section) {}

std::string Student::getRoleName() const {
    return "STUDENT";
}

std::string Student::toJson() const {
    return User::toJson();
}

std::string Student::toFileString() const {
    return User::toFileString();
}

void Student::printDetails() const {
    User::printDetails();
}

// ============================================================================
// Class Representative Implementation (Derived Class 2)
// ============================================================================
ClassRepresentative::ClassRepresentative()
    : User(), term("2026-2027") {}

ClassRepresentative::ClassRepresentative(const std::string& id, const std::string& username, const std::string& password,
                                         const std::string& fullName, const std::string& rollNumber, const std::string& enrollmentNumber,
                                         int semester, const std::string& branch, const std::string& section,
                                         const std::string& term)
    : User(id, username, password, fullName, rollNumber, enrollmentNumber, semester, branch, section),
      term(term) {}

std::string ClassRepresentative::getRoleName() const {
    return "CR";
}

std::string ClassRepresentative::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << "\"id\":\"" << CommonUtils::escapeJson(id) << "\","
       << "\"username\":\"" << CommonUtils::escapeJson(username) << "\","
       << "\"fullName\":\"" << CommonUtils::escapeJson(fullName) << "\","
       << "\"rollNumber\":\"" << CommonUtils::escapeJson(rollNumber) << "\","
       << "\"enrollmentNumber\":\"" << CommonUtils::escapeJson(enrollmentNumber) << "\","
       << "\"semester\":" << semester << ","
       << "\"branch\":\"" << CommonUtils::escapeJson(branch) << "\","
       << "\"section\":\"" << CommonUtils::escapeJson(section) << "\","
       << "\"term\":\"" << CommonUtils::escapeJson(term) << "\","
       << "\"role\":\"CR\","
       << "\"canManageAssignments\":true,"
       << "\"canDeclareHolidays\":true"
       << "}";
    return ss.str();
}

std::string ClassRepresentative::toFileString() const {
    return User::toFileString() + "|" + term;
}

void ClassRepresentative::printDetails() const {
    User::printDetails();
    std::cout << "CR Tenure  : " << term << "\n";
}

// ============================================================================
// UserManager Implementation
// ============================================================================
UserManager::UserManager(const std::string& filePath) : dataFilePath(filePath) {
    if (!loadFromFile() || users.empty()) {
        initializeDefaultUsers();
        saveToFile();
    }
}

UserManager::~UserManager() {
    clearUsers();
}

void UserManager::clearUsers() {
    for (size_t i = 0; i < users.size(); ++i) {
        delete users[i];
    }
    users.clear();
}

void UserManager::initializeDefaultUsers() {
    clearUsers();
    // 1. Class Representative (CR) - Priyanshu Raj
    users.push_back(new ClassRepresentative(
        "u_cr01", "priyanshu_cr", "priyanshu123",
        "Priyanshu Raj (CR)", "23ITB01", "0101IT231001",
        3, "Information Technology", "IT-B", "2026-2027"
    ));

    // 2. Classmate 1 - Prakash Kumar
    users.push_back(new Student(
        "u_st01", "prakash", "prakash123",
        "Prakash Kumar", "23ITB25", "0101IT231025",
        3, "Information Technology", "IT-B"
    ));

    // 3. Classmate 2 - Ravikant
    users.push_back(new Student(
        "u_st02", "ravikant", "ravi123",
        "Ravikant", "23ITB38", "0101IT231038",
        3, "Information Technology", "IT-B"
    ));
}

bool UserManager::loadFromFile() {
    std::ifstream inFile(dataFilePath.c_str());
    if (!inFile.is_open()) return false;

    clearUsers();
    std::string line;
    while (std::getline(inFile, line)) {
        line = CommonUtils::trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 10) {
            std::string role = tokens[0];
            std::string id = tokens[1];
            std::string username = tokens[2];
            std::string password = tokens[3];
            std::string fullName = tokens[4];
            std::string rollNumber = tokens[5];
            std::string enrollmentNumber = tokens[6];
            int semester = 3;
            try { semester = std::stoi(tokens[7]); } catch (...) { semester = 3; }
            std::string branch = tokens[8];
            std::string section = tokens[9];

            if (role == "CR") {
                std::string term = (tokens.size() > 10) ? tokens[10] : "2026-2027";
                users.push_back(new ClassRepresentative(id, username, password, fullName, rollNumber, enrollmentNumber, semester, branch, section, term));
            } else {
                users.push_back(new Student(id, username, password, fullName, rollNumber, enrollmentNumber, semester, branch, section));
            }
        }
    }
    inFile.close();
    return true;
}

bool UserManager::saveToFile() const {
    std::ofstream outFile(dataFilePath.c_str());
    if (!outFile.is_open()) return false;

    outFile << "# Role|ID|Username|Password|FullName|RollNumber|Enrollment|Semester|Branch|Section|Extra\n";
    for (size_t i = 0; i < users.size(); ++i) {
        outFile << users[i]->toFileString() << "\n";
    }
    outFile.close();
    return true;
}

User* UserManager::authenticate(const std::string& username, const std::string& password) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i]->getUsername() == username && users[i]->verifyPassword(password)) {
            return users[i];
        }
    }
    return NULL;
}

User* UserManager::findById(const std::string& id) const {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i]->getId() == id) return users[i];
    }
    return NULL;
}

User* UserManager::findByUsername(const std::string& username) const {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i]->getUsername() == username) return users[i];
    }
    return NULL;
}

bool UserManager::updateProfile(const std::string& userId, const std::string& fullName,
                               const std::string& rollNumber, const std::string& enrollmentNumber,
                               int semester, const std::string& branch, const std::string& section) {
    User* u = findById(userId);
    if (!u) return false;

    if (!fullName.empty()) u->setFullName(fullName);
    if (!rollNumber.empty()) u->setRollNumber(rollNumber);
    if (!enrollmentNumber.empty()) u->setEnrollmentNumber(enrollmentNumber);
    if (semester > 0) u->setSemester(semester);
    if (!branch.empty()) u->setBranch(branch);
    if (!section.empty()) u->setSection(section);

    saveToFile();
    return true;
}

bool UserManager::registerStudent(const std::string& username, const std::string& password,
                                 const std::string& fullName, const std::string& rollNumber,
                                 const std::string& enrollmentNumber, int semester,
                                 const std::string& branch, const std::string& section) {
    if (findByUsername(username) != NULL) {
        return false;
    }

    std::string newId = "u_st" + std::to_string(users.size() + 1);
    User* newStudent = new Student(newId, username, password, fullName, rollNumber, enrollmentNumber, semester, branch, section);
    users.push_back(newStudent);
    saveToFile();
    return true;
}

std::string UserManager::getAllUsersJson() const {
    std::ostringstream ss;
    ss << "[";
    for (size_t i = 0; i < users.size(); ++i) {
        if (i > 0) ss << ",";
        ss << users[i]->toJson();
    }
    ss << "]";
    return ss.str();
}
