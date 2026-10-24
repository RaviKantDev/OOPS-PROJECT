#ifndef MEMBER3_ATTENDANCE_TRACKER_H
#define MEMBER3_ATTENDANCE_TRACKER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "CommonUtils.h"

// ============================================================================
// PART 3 (DEVELOPED BY TEAM MEMBER 3):
// SMART ATTENDANCE TRACKER, HOLIDAY ENGINE & DYNAMIC SUBJECT MANAGER
// Designed for B.Tech IT-B 3rd Semester
// 
// OOP Concepts Demonstrated:
// 1. Custom Exception Handling Hierarchy (AppException -> FutureDateException, WeekendException, HolidayException)
// 2. Class Encapsulation & Separation of Concerns (AttendanceRecord, HolidayCalendar, SubjectAttendanceStats)
// 3. Mathematical Modeling & Predictive Business Logic (Safe Misses & Recovery calculations for 75% and 65%)
// 4. Robust Validation Pipelines (Strict prevention of future dates, weekends, and CR holidays)
// 5. File I/O Persistence (managing attendance records, custom subjects, and holiday lists on disk)
// ============================================================================

// Model representing a single attendance log entry
class AttendanceRecord {
private:
    std::string id;
    std::string studentId;
    std::string subjectName;
    std::string date;       // Format: YYYY-MM-DD
    std::string status;     // "PRESENT", "ABSENT", "CANCELLED"
    std::string timestamp;

public:
    AttendanceRecord();
    AttendanceRecord(const std::string& id, const std::string& studentId,
                     const std::string& subjectName, const std::string& date,
                     const std::string& status, const std::string& timestamp = "");

    std::string getId() const { return id; }
    std::string getStudentId() const { return studentId; }
    std::string getSubjectName() const { return subjectName; }
    std::string getDate() const { return date; }
    std::string getStatus() const { return status; }
    std::string getTimestamp() const { return timestamp; }

    void setStatus(const std::string& s) { status = s; }

    std::string toJson() const;
    std::string toFileString() const;
    static AttendanceRecord fromFileString(const std::string& line);
};

// Represents an official college holiday declared by Class Representative (CR)
class CollegeHoliday {
private:
    std::string date;        // Format: YYYY-MM-DD
    std::string description; // e.g. "Diwali Break", "Sports Day", "Tech Fest"
    std::string declaredBy;  // CR User ID or Name

public:
    CollegeHoliday();
    CollegeHoliday(const std::string& date, const std::string& description, const std::string& declaredBy = "Priyanshu Raj (CR)");

    std::string getDate() const { return date; }
    std::string getDescription() const { return description; }
    std::string getDeclaredBy() const { return declaredBy; }

    std::string toJson() const;
    std::string toFileString() const;
    static CollegeHoliday fromFileString(const std::string& line);
};

// Calculated analytics for a single subject
struct SubjectAttendanceStats {
    std::string subjectName;
    int attendedCount;      // Number of classes attended (PRESENT)
    int absentCount;        // Number of classes missed (ABSENT)
    int cancelledCount;     // Classes cancelled by department
    int totalHeld;          // attendedCount + absentCount (cancelled classes do not penalize)
    double percentage;      // (attendedCount / totalHeld) * 100
    
    // Predictive Analytics for 75% Requirement
    int safeMisses75;       // Number of classes student can safely bunk while staying >= 75%
    int requiredAttend75;   // If < 75%, consecutive classes required to reach 75%
    
    // Predictive Analytics for 65% Requirement (Medical / Condonation Minimum)
    int safeMisses65;       // Number of classes student can safely bunk while staying >= 65%
    int requiredAttend65;   // If < 65%, consecutive classes required to reach 65%

    std::string statusTag;  // "Safe", "Warning", "Critical"

    std::string toJson() const;
};

// Holiday Calendar Manager (Fixed Weekends + CR Declared Holidays)
class HolidayManager {
private:
    std::vector<CollegeHoliday> customHolidays;
    std::string filePath;

public:
    HolidayManager(const std::string& path = "data/holidays.txt");

    bool loadFromFile();
    bool saveToFile() const;
    void initializeDefaultHolidays();

    // Verification methods
    bool isCustomHoliday(const std::string& dateStr, std::string& outReason) const;
    bool isWeekend(const std::string& dateStr, std::string& outDayName) const;
    
    // Full date check: Throws custom exceptions if invalid
    void validateDateForAttendance(const std::string& dateStr) const;

    // CR Management methods
    bool addHoliday(const std::string& dateStr, const std::string& description, const std::string& declaredBy = "Priyanshu Raj (CR)");
    bool removeHoliday(const std::string& dateStr);

    const std::vector<CollegeHoliday>& getAllHolidays() const { return customHolidays; }
    std::string getAllHolidaysJson() const;
};

// Overall Attendance Tracker Engine with Dynamic Subject Additions
class AttendanceTracker {
private:
    std::vector<AttendanceRecord> records;
    std::string dataFilePath;
    std::string customSubjectsFilePath;
    HolidayManager holidayManager;
    std::vector<std::string> defaultSubjects;
    std::map<std::string, std::vector<std::string>> studentCustomSubjects;

    void initializeDefaultRecords();
    bool loadCustomSubjects();
    bool saveCustomSubjects() const;

public:
    AttendanceTracker(const std::string& attendancePath = "data/attendance.txt",
                      const std::string& holidaysPath = "data/holidays.txt",
                      const std::string& subjectsPath = "data/student_subjects.txt");

    bool loadFromFile();
    bool saveToFile() const;

    // Subject Management per student
    std::vector<std::string> getSubjectsForStudent(const std::string& studentId) const;
    bool addCustomSubject(const std::string& studentId, const std::string& subjectName);

    // Attendance recording with strict validation checks
    // Throws: FutureDateException, WeekendException, HolidayException
    bool markAttendance(const std::string& studentId, const std::string& subjectName,
                        const std::string& dateStr, const std::string& status);

    // Analytics and calculations
    SubjectAttendanceStats calculateStatsForSubject(const std::string& studentId, const std::string& subjectName) const;
    std::vector<SubjectAttendanceStats> getAllSubjectStats(const std::string& studentId) const;

    // History and queries
    std::vector<AttendanceRecord> getStudentRecords(const std::string& studentId) const;
    std::vector<AttendanceRecord> getStudentRecordsForDate(const std::string& studentId, const std::string& dateStr) const;

    // Holiday management delegation
    HolidayManager& getHolidayManager() { return holidayManager; }
    const HolidayManager& getHolidayManager() const { return holidayManager; }

    // JSON export
    std::string getStudentDashboardJson(const std::string& studentId) const;
};

#endif // MEMBER3_ATTENDANCE_TRACKER_H
