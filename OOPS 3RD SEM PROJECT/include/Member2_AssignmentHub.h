#ifndef MEMBER2_ASSIGNMENT_HUB_H
#define MEMBER2_ASSIGNMENT_HUB_H

#include <string>
#include <vector>
#include <iostream>

// ============================================================================
// PART 2 (DEVELOPED BY TEAM MEMBER 2):
// ASSIGNMENT MANAGEMENT HUB & SUBMISSION MEDIUM TRACKER
// 
// OOP Concepts Demonstrated:
// 1. Class Design & Data Encapsulation (attributes private, access via getters/setters)
// 2. Operator Overloading (operator< for deadline comparison, operator<< for printing)
// 3. Object Composition (AssignmentManager manages collections of Assignment objects)
// 4. File I/O Persistence (reading/writing formatted assignment records to disk)
// 5. Data Filtering & Sorting Algorithms (subject, submission medium, deadline)
// ============================================================================

class Assignment {
private:
    std::string id;
    std::string subjectCode;
    std::string subjectName;
    std::string title;
    std::string description;
    std::string assignedDate;       // Format: YYYY-MM-DD
    std::string deadlineDate;       // Format: YYYY-MM-DD
    std::string submissionMedium;   // e.g., "Assignment Copy", "Stick File", "Loose Sheets", "Spiral File"
    std::string facultyName;
    int maxMarks;
    std::string instructions;

public:
    // Constructors
    Assignment();
    Assignment(const std::string& id, const std::string& subjectCode, const std::string& subjectName,
               const std::string& title, const std::string& description,
               const std::string& assignedDate, const std::string& deadlineDate,
               const std::string& submissionMedium, const std::string& facultyName = "Faculty",
               int maxMarks = 20, const std::string& instructions = "Submit on time");

    // Getters
    std::string getId() const { return id; }
    std::string getSubjectCode() const { return subjectCode; }
    std::string getSubjectName() const { return subjectName; }
    std::string getTitle() const { return title; }
    std::string getDescription() const { return description; }
    std::string getAssignedDate() const { return assignedDate; }
    std::string getDeadlineDate() const { return deadlineDate; }
    std::string getSubmissionMedium() const { return submissionMedium; }
    std::string getFacultyName() const { return facultyName; }
    int getMaxMarks() const { return maxMarks; }
    std::string getInstructions() const { return instructions; }

    // Setters
    void setSubjectCode(const std::string& code) { subjectCode = code; }
    void setSubjectName(const std::string& name) { subjectName = name; }
    void setTitle(const std::string& t) { title = t; }
    void setDescription(const std::string& d) { description = d; }
    void setAssignedDate(const std::string& date) { assignedDate = date; }
    void setDeadlineDate(const std::string& date) { deadlineDate = date; }
    void setSubmissionMedium(const std::string& medium) { submissionMedium = medium; }
    void setFacultyName(const std::string& faculty) { facultyName = faculty; }
    void setMaxMarks(int marks) { maxMarks = marks; }
    void setInstructions(const std::string& inst) { instructions = inst; }

    // OOP CONCEPT: Operator Overloading
    // Overloading '<' to automatically sort assignments by deadline date
    bool operator<(const Assignment& other) const {
        return this->deadlineDate < other.deadlineDate;
    }

    // Overloading '==' to check equality by assignment ID
    bool operator==(const Assignment& other) const {
        return this->id == other.id;
    }

    // Overloading stream insertion operator '<<' for clean C++ console output
    friend std::ostream& operator<<(std::ostream& os, const Assignment& a);

    // Serialization
    std::string toJson() const;
    std::string toFileString() const;
    static Assignment fromFileString(const std::string& line);
};

// Manager class encapsulating all Assignment operations
class AssignmentManager {
private:
    std::vector<Assignment> assignments;
    std::string dataFilePath;

public:
    AssignmentManager(const std::string& filePath = "data/assignments.txt");

    // File Persistence
    bool loadFromFile();
    bool saveToFile() const;
    void initializeDefaultAssignments();

    // CRUD Operations (Class Representative functions)
    bool addAssignment(const Assignment& asgn);
    bool updateAssignment(const std::string& id, const Assignment& updatedAsgn);
    bool deleteAssignment(const std::string& id);

    // Query & Filter Operations (Used by Classmates and CR)
    const std::vector<Assignment>& getAllAssignments() const { return assignments; }
    Assignment* getAssignmentById(const std::string& id);
    std::vector<Assignment> getAssignmentsBySubject(const std::string& subjectName) const;
    std::vector<Assignment> getAssignmentsByMedium(const std::string& medium) const;
    std::vector<Assignment> getSortedAssignments() const;

    // Web API representation
    std::string getAllAssignmentsJson() const;
};

#endif // MEMBER2_ASSIGNMENT_HUB_H

