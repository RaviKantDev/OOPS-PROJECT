#include "../include/Member2_AssignmentHub.h"
#include "../include/CommonUtils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>

Assignment::Assignment()
    : id(""), subjectCode(""), subjectName(""), title(""), description(""),
      assignedDate(""), deadlineDate(""), submissionMedium("Assignment Copy"),
      facultyName("Faculty"), maxMarks(20), instructions("Submit before deadline") {}

Assignment::Assignment(const std::string& id, const std::string& subjectCode, const std::string& subjectName,
                       const std::string& title, const std::string& description,
                       const std::string& assignedDate, const std::string& deadlineDate,
                       const std::string& submissionMedium, const std::string& facultyName,
                       int maxMarks, const std::string& instructions)
    : id(id), subjectCode(subjectCode), subjectName(subjectName),
      title(title), description(description),
      assignedDate(assignedDate), deadlineDate(deadlineDate),
      submissionMedium(submissionMedium), facultyName(facultyName),
      maxMarks(maxMarks), instructions(instructions) {}

std::ostream& operator<<(std::ostream& os, const Assignment& a) {
    os << "--------------------------------------------------------\n"
       << "[" << a.subjectCode << "] " << a.subjectName << "\n"
       << "Title            : " << a.title << "\n"
       << "Submission Medium: " << a.submissionMedium << "\n"
       << "Assigned Date    : " << a.assignedDate << "\n"
       << "Deadline Date    : " << a.deadlineDate << "\n"
       << "Faculty          : " << a.facultyName << " | Max Marks: " << a.maxMarks << "\n"
       << "Instructions     : " << a.instructions << "\n"
       << "--------------------------------------------------------\n";
    return os;
}

std::string Assignment::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << "\"id\":\"" << CommonUtils::escapeJson(id) << "\","
       << "\"subjectCode\":\"" << CommonUtils::escapeJson(subjectCode) << "\","
       << "\"subjectName\":\"" << CommonUtils::escapeJson(subjectName) << "\","
       << "\"title\":\"" << CommonUtils::escapeJson(title) << "\","
       << "\"description\":\"" << CommonUtils::escapeJson(description) << "\","
       << "\"assignedDate\":\"" << CommonUtils::escapeJson(assignedDate) << "\","
       << "\"deadlineDate\":\"" << CommonUtils::escapeJson(deadlineDate) << "\","
       << "\"submissionMedium\":\"" << CommonUtils::escapeJson(submissionMedium) << "\","
       << "\"facultyName\":\"" << CommonUtils::escapeJson(facultyName) << "\","
       << "\"maxMarks\":" << maxMarks << ","
       << "\"instructions\":\"" << CommonUtils::escapeJson(instructions) << "\""
       << "}";
    return ss.str();
}

std::string Assignment::toFileString() const {
    std::ostringstream ss;
    ss << id << "|"
       << subjectCode << "|"
       << subjectName << "|"
       << title << "|"
       << description << "|"
       << assignedDate << "|"
       << deadlineDate << "|"
       << submissionMedium << "|"
       << facultyName << "|"
       << maxMarks << "|"
       << instructions;
    return ss.str();
}

Assignment Assignment::fromFileString(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    if (tokens.size() >= 8) {
        std::string id = tokens[0];
        std::string code = tokens[1];
        std::string subject = tokens[2];
        std::string title = tokens[3];
        std::string desc = tokens[4];
        std::string assigned = tokens[5];
        std::string deadline = tokens[6];
        std::string medium = tokens[7];
        std::string faculty = (tokens.size() > 8) ? tokens[8] : "Faculty";
        int marks = (tokens.size() > 9) ? std::stoi(tokens[9]) : 20;
        std::string instructions = (tokens.size() > 10) ? tokens[10] : "Submit on time";
        return Assignment(id, code, subject, title, desc, assigned, deadline, medium, faculty, marks, instructions);
    }
    return Assignment();
}

// ============================================================================
// AssignmentManager Implementation
// ============================================================================
AssignmentManager::AssignmentManager(const std::string& filePath) : dataFilePath(filePath) {
    if (!loadFromFile() || assignments.empty()) {
        initializeDefaultAssignments();
        saveToFile();
    }
}

void AssignmentManager::initializeDefaultAssignments() {
    assignments.clear();
    
    // Assignment 1
    assignments.push_back(Assignment(
        "asgn_101", "IT301", "Object Oriented Programming (C++)",
        "Polymorphism, Virtual Functions & Exception Handling",
        "Implement base class User with dynamic dispatch methods. Write complete report with class hierarchy diagrams.",
        "2026-10-20", "2026-10-29",
        "Practical Copy",
        "Dr. Rajesh Kumar", 25, "Handwritten on practical lab copy with blue/black pen only."
    ));

    // Assignment 2
    assignments.push_back(Assignment(
        "asgn_102", "IT302", "Data Structures & Algorithms",
        "AVL Tree Balancing & Dijkstra Shortest Path Trace",
        "Solve the 5 problems on balancing factors and write step-by-step trace of Dijkstra's algorithm for given graph.",
        "2026-10-22", "2026-10-31",
        "Stick File",
        "Prof. Anita Sen", 20, "Punch A4 ruled sheets inside a standard stick file. Write Roll No on top-right."
    ));

    // Assignment 3
    assignments.push_back(Assignment(
        "asgn_103", "IT303", "Computer Organization & Architecture",
        "Booth Multiplication & Cache Mapping Problems",
        "Solve numericals on 2-way set associative cache hit/miss and trace 8-bit Booth multiplication algorithm.",
        "2026-10-18", "2026-10-28",
        "Assignment Copy",
        "Dr. S. K. Mitra", 20, "Write in IT-B dedicated assignment register."
    ));

    // Assignment 4
    assignments.push_back(Assignment(
        "asgn_104", "IT304", "Discrete Mathematics & Graph Theory",
        "Recurrence Relations & Planar Graphs Tutorial #4",
        "Solve problems 1 to 12 from tutorial sheet 4 on second order homogeneous recurrence relations.",
        "2026-10-21", "2026-11-03",
        "Loose Graph Sheets",
        "Prof. Manoj Tripathi", 20, "Submit 2 graph sheets clamped together with header slip."
    ));

    // Assignment 5
    assignments.push_back(Assignment(
        "asgn_105", "IT305", "Digital Logic & Design",
        "Mod-10 Synchronous Counter Design & State Diagram",
        "Design synchronous Mod-10 counter using JK Flip-Flops. Draw complete circuit diagram and truth table.",
        "2026-10-23", "2026-11-06",
        "Spiral File",
        "Dr. Neha Kapoor", 20, "Spiral bound with printed cover page and signed acknowledgment."
    ));
}

bool AssignmentManager::loadFromFile() {
    std::ifstream inFile(dataFilePath.c_str());
    if (!inFile.is_open()) return false;

    assignments.clear();
    std::string line;
    while (std::getline(inFile, line)) {
        line = CommonUtils::trim(line);
        if (line.empty() || line[0] == '#') continue;
        Assignment a = Assignment::fromFileString(line);
        if (!a.getId().empty()) {
            assignments.push_back(a);
        }
    }
    inFile.close();
    return true;
}

bool AssignmentManager::saveToFile() const {
    std::ofstream outFile(dataFilePath.c_str());
    if (!outFile.is_open()) return false;

    outFile << "# ID|SubjectCode|SubjectName|Title|Description|AssignedDate|DeadlineDate|Medium|Faculty|Marks|Instructions\n";
    for (size_t i = 0; i < assignments.size(); ++i) {
        outFile << assignments[i].toFileString() << "\n";
    }
    outFile.close();
    return true;
}

bool AssignmentManager::addAssignment(const Assignment& asgn) {
    Assignment newAsgn = asgn;
    if (newAsgn.getId().empty()) {
        newAsgn = Assignment(
            "asgn_" + std::to_string(std::time(NULL) % 100000),
            asgn.getSubjectCode(), asgn.getSubjectName(), asgn.getTitle(),
            asgn.getDescription(), asgn.getAssignedDate(), asgn.getDeadlineDate(),
            asgn.getSubmissionMedium(), asgn.getFacultyName(), asgn.getMaxMarks(),
            asgn.getInstructions()
        );
    }
    assignments.push_back(newAsgn);
    saveToFile();
    return true;
}

bool AssignmentManager::updateAssignment(const std::string& id, const Assignment& updatedAsgn) {
    for (size_t i = 0; i < assignments.size(); ++i) {
        if (assignments[i].getId() == id) {
            assignments[i] = updatedAsgn;
            saveToFile();
            return true;
        }
    }
    return false;
}

bool AssignmentManager::deleteAssignment(const std::string& id) {
    for (size_t i = 0; i < assignments.size(); ++i) {
        if (assignments[i].getId() == id) {
            assignments.erase(assignments.begin() + i);
            saveToFile();
            return true;
        }
    }
    return false;
}

Assignment* AssignmentManager::getAssignmentById(const std::string& id) {
    for (size_t i = 0; i < assignments.size(); ++i) {
        if (assignments[i].getId() == id) {
            return &assignments[i];
        }
    }
    return NULL;
}

std::vector<Assignment> AssignmentManager::getAssignmentsBySubject(const std::string& subjectName) const {
    std::vector<Assignment> filtered;
    for (size_t i = 0; i < assignments.size(); ++i) {
        if (assignments[i].getSubjectName().find(subjectName) != std::string::npos ||
            assignments[i].getSubjectCode().find(subjectName) != std::string::npos) {
            filtered.push_back(assignments[i]);
        }
    }
    return filtered;
}

std::vector<Assignment> AssignmentManager::getAssignmentsByMedium(const std::string& medium) const {
    std::vector<Assignment> filtered;
    for (size_t i = 0; i < assignments.size(); ++i) {
        if (assignments[i].getSubmissionMedium() == medium) {
            filtered.push_back(assignments[i]);
        }
    }
    return filtered;
}

std::vector<Assignment> AssignmentManager::getSortedAssignments() const {
    std::vector<Assignment> sorted = assignments;
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

std::string AssignmentManager::getAllAssignmentsJson() const {
    std::vector<Assignment> sorted = getSortedAssignments();
    std::ostringstream ss;
    ss << "[";
    for (size_t i = 0; i < sorted.size(); ++i) {
        if (i > 0) ss << ",";
        ss << sorted[i].toJson();
    }
    ss << "]";
    return ss.str();
}
