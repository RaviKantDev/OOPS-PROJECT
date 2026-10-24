#include "../include/Member3_AttendanceTracker.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <algorithm>

// ============================================================================
// AttendanceRecord Implementation
// ============================================================================
AttendanceRecord::AttendanceRecord()
    : id(""), studentId(""), subjectName(""), date(""), status("PRESENT"), timestamp("") {}

AttendanceRecord::AttendanceRecord(const std::string& id, const std::string& studentId,
                                   const std::string& subjectName, const std::string& date,
                                   const std::string& status, const std::string& timestamp)
    : id(id), studentId(studentId), subjectName(subjectName),
      date(date), status(status), timestamp(timestamp) {}

std::string AttendanceRecord::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << "\"id\":\"" << CommonUtils::escapeJson(id) << "\","
       << "\"studentId\":\"" << CommonUtils::escapeJson(studentId) << "\","
       << "\"subjectName\":\"" << CommonUtils::escapeJson(subjectName) << "\","
       << "\"date\":\"" << CommonUtils::escapeJson(date) << "\","
       << "\"status\":\"" << CommonUtils::escapeJson(status) << "\","
       << "\"timestamp\":\"" << CommonUtils::escapeJson(timestamp) << "\""
       << "}";
    return ss.str();
}

std::string AttendanceRecord::toFileString() const {
    std::ostringstream ss;
    ss << id << "|"
       << studentId << "|"
       << subjectName << "|"
       << date << "|"
       << status << "|"
       << timestamp;
    return ss.str();
}

AttendanceRecord AttendanceRecord::fromFileString(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    if (tokens.size() >= 5) {
        std::string id = tokens[0];
        std::string studentId = tokens[1];
        std::string subject = tokens[2];
        std::string date = tokens[3];
        std::string status = tokens[4];
        std::string ts = (tokens.size() > 5) ? tokens[5] : "";
        return AttendanceRecord(id, studentId, subject, date, status, ts);
    }
    return AttendanceRecord();
}

// ============================================================================
// CollegeHoliday Implementation
// ============================================================================
CollegeHoliday::CollegeHoliday() : date(""), description(""), declaredBy("Priyanshu Raj (CR)") {}

CollegeHoliday::CollegeHoliday(const std::string& date, const std::string& description, const std::string& declaredBy)
    : date(date), description(description), declaredBy(declaredBy) {}

std::string CollegeHoliday::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << "\"date\":\"" << CommonUtils::escapeJson(date) << "\","
       << "\"description\":\"" << CommonUtils::escapeJson(description) << "\","
       << "\"declaredBy\":\"" << CommonUtils::escapeJson(declaredBy) << "\""
       << "}";
    return ss.str();
}

std::string CollegeHoliday::toFileString() const {
    return date + "|" + description + "|" + declaredBy;
}

CollegeHoliday CollegeHoliday::fromFileString(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    if (tokens.size() >= 2) {
        std::string d = tokens[0];
        std::string desc = tokens[1];
        std::string by = (tokens.size() > 2) ? tokens[2] : "Priyanshu Raj (CR)";
        return CollegeHoliday(d, desc, by);
    }
    return CollegeHoliday();
}

// ============================================================================
// SubjectAttendanceStats Implementation
// ============================================================================
std::string SubjectAttendanceStats::toJson() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1);
    ss << "{"
       << "\"subjectName\":\"" << CommonUtils::escapeJson(subjectName) << "\","
       << "\"attendedCount\":" << attendedCount << ","
       << "\"absentCount\":" << absentCount << ","
       << "\"cancelledCount\":" << cancelledCount << ","
       << "\"totalHeld\":" << totalHeld << ","
       << "\"percentage\":" << percentage << ","
       << "\"safeMisses75\":" << safeMisses75 << ","
       << "\"requiredAttend75\":" << requiredAttend75 << ","
       << "\"safeMisses65\":" << safeMisses65 << ","
       << "\"requiredAttend65\":" << requiredAttend65 << ","
       << "\"statusTag\":\"" << statusTag << "\""
       << "}";
    return ss.str();
}

// ============================================================================
// HolidayManager Implementation
// ============================================================================
HolidayManager::HolidayManager(const std::string& path) : filePath(path) {
    if (!loadFromFile() || customHolidays.empty()) {
        initializeDefaultHolidays();
        saveToFile();
    }
}

void HolidayManager::initializeDefaultHolidays() {
    customHolidays.clear();
    customHolidays.push_back(CollegeHoliday("2026-10-14", "Diwali Pre-Break & Cultural Fest", "Priyanshu Raj (CR)"));
    customHolidays.push_back(CollegeHoliday("2026-10-30", "Annual IT Department Sports Meet", "Priyanshu Raj (CR)"));
    customHolidays.push_back(CollegeHoliday("2026-11-04", "University Foundation Day", "Priyanshu Raj (CR)"));
}

bool HolidayManager::loadFromFile() {
    std::ifstream inFile(filePath.c_str());
    if (!inFile.is_open()) return false;

    customHolidays.clear();
    std::string line;
    while (std::getline(inFile, line)) {
        line = CommonUtils::trim(line);
        if (line.empty() || line[0] == '#') continue;
        CollegeHoliday h = CollegeHoliday::fromFileString(line);
        if (!h.getDate().empty()) {
            customHolidays.push_back(h);
        }
    }
    inFile.close();
    return true;
}

bool HolidayManager::saveToFile() const {
    std::ofstream outFile(filePath.c_str());
    if (!outFile.is_open()) return false;

    outFile << "# Date|Description|DeclaredBy\n";
    for (size_t i = 0; i < customHolidays.size(); ++i) {
        outFile << customHolidays[i].toFileString() << "\n";
    }
    outFile.close();
    return true;
}

bool HolidayManager::isCustomHoliday(const std::string& dateStr, std::string& outReason) const {
    for (size_t i = 0; i < customHolidays.size(); ++i) {
        if (customHolidays[i].getDate() == dateStr) {
            outReason = customHolidays[i].getDescription();
            return true;
        }
    }
    return false;
}

bool HolidayManager::isWeekend(const std::string& dateStr, std::string& outDayName) const {
    if (CommonUtils::isWeekend(dateStr)) {
        outDayName = CommonUtils::getDayName(dateStr);
        return true;
    }
    return false;
}

void HolidayManager::validateDateForAttendance(const std::string& dateStr) const {
    // 1. Strict check: Can't mark upcoming/future dates
    if (CommonUtils::isFutureDate(dateStr)) {
        throw FutureDateException(dateStr);
    }

    // 2. Strict check: Saturday and Sunday are always fixed holidays
    std::string dayName;
    if (isWeekend(dateStr, dayName)) {
        throw WeekendException(dateStr, dayName);
    }

    // 3. Strict check: CR declared official college holidays
    std::string reason;
    if (isCustomHoliday(dateStr, reason)) {
        throw HolidayException(dateStr, reason);
    }
}

bool HolidayManager::addHoliday(const std::string& dateStr, const std::string& description, const std::string& declaredBy) {
    std::string reason;
    if (isCustomHoliday(dateStr, reason)) {
        return false;
    }
    customHolidays.push_back(CollegeHoliday(dateStr, description, declaredBy));
    saveToFile();
    return true;
}

bool HolidayManager::removeHoliday(const std::string& dateStr) {
    for (size_t i = 0; i < customHolidays.size(); ++i) {
        if (customHolidays[i].getDate() == dateStr) {
            customHolidays.erase(customHolidays.begin() + i);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::string HolidayManager::getAllHolidaysJson() const {
    std::ostringstream ss;
    ss << "[";
    for (size_t i = 0; i < customHolidays.size(); ++i) {
        if (i > 0) ss << ",";
        ss << customHolidays[i].toJson();
    }
    ss << "]";
    return ss.str();
}

// ============================================================================
// AttendanceTracker Implementation
// ============================================================================
AttendanceTracker::AttendanceTracker(const std::string& attendancePath,
                                     const std::string& holidaysPath,
                                     const std::string& subjectsPath)
    : dataFilePath(attendancePath), customSubjectsFilePath(subjectsPath), holidayManager(holidaysPath) {
    
    // Core B.Tech IT-B 3rd Sem subjects
    defaultSubjects.push_back("Object Oriented Programming (C++)");
    defaultSubjects.push_back("Data Structures & Algorithms");
    defaultSubjects.push_back("Computer Organization & Architecture");
    defaultSubjects.push_back("Discrete Mathematics & Graph Theory");
    defaultSubjects.push_back("Digital Logic & Design");

    loadCustomSubjects();

    if (!loadFromFile() || records.empty()) {
        initializeDefaultRecords();
        saveToFile();
    }
}

bool AttendanceTracker::loadCustomSubjects() {
    std::ifstream inFile(customSubjectsFilePath.c_str());
    if (!inFile.is_open()) return false;

    studentCustomSubjects.clear();
    std::string line;
    while (std::getline(inFile, line)) {
        line = CommonUtils::trim(line);
        if (line.empty() || line[0] == '#') continue;

        size_t pipe = line.find('|');
        if (pipe != std::string::npos) {
            std::string stId = line.substr(0, pipe);
            std::string sub = line.substr(pipe + 1);
            studentCustomSubjects[stId].push_back(sub);
        }
    }
    inFile.close();
    return true;
}

bool AttendanceTracker::saveCustomSubjects() const {
    std::ofstream outFile(customSubjectsFilePath.c_str());
    if (!outFile.is_open()) return false;

    outFile << "# StudentID|CustomSubjectName\n";
    for (auto it = studentCustomSubjects.begin(); it != studentCustomSubjects.end(); ++it) {
        const std::string& stId = it->first;
        const std::vector<std::string>& list = it->second;
        for (size_t i = 0; i < list.size(); ++i) {
            outFile << stId << "|" << list[i] << "\n";
        }
    }
    outFile.close();
    return true;
}

std::vector<std::string> AttendanceTracker::getSubjectsForStudent(const std::string& studentId) const {
    std::vector<std::string> combined = defaultSubjects;
    auto it = studentCustomSubjects.find(studentId);
    if (it != studentCustomSubjects.end()) {
        const std::vector<std::string>& extra = it->second;
        for (size_t i = 0; i < extra.size(); ++i) {
            if (std::find(combined.begin(), combined.end(), extra[i]) == combined.end()) {
                combined.push_back(extra[i]);
            }
        }
    }
    return combined;
}

bool AttendanceTracker::addCustomSubject(const std::string& studentId, const std::string& subjectName) {
    std::string trimmed = CommonUtils::trim(subjectName);
    if (trimmed.empty()) return false;

    std::vector<std::string> current = getSubjectsForStudent(studentId);
    if (std::find(current.begin(), current.end(), trimmed) != current.end()) {
        return false; // Already enrolled/added
    }

    studentCustomSubjects[studentId].push_back(trimmed);
    saveCustomSubjects();
    return true;
}

void AttendanceTracker::initializeDefaultRecords() {
    records.clear();
    std::string dates[] = {"2026-10-19", "2026-10-20", "2026-10-21", "2026-10-22", "2026-10-23"};
    std::string students[] = {"u_st01", "u_st02", "u_cr01"};

    int count = 1;
    for (int stIdx = 0; stIdx < 3; ++stIdx) {
        std::string currentSt = students[stIdx];
        for (int d = 0; d < 5; ++d) {
            for (size_t s = 0; s < defaultSubjects.size(); ++s) {
                std::string st = "PRESENT";
                if (stIdx == 0 && s == 2 && d == 2) st = "ABSENT"; // Prakash missed COA once
                if (stIdx == 0 && s == 3 && d == 4) st = "ABSENT"; // Prakash missed Maths once
                if (stIdx == 1 && s == 1 && d == 1) st = "ABSENT"; // Ravikant missed DSA once
                if (stIdx == 1 && s == 4 && d == 3) st = "CANCELLED"; // Digital Logic cancelled

                std::string recId = "att_" + std::to_string(count++);
                records.push_back(AttendanceRecord(recId, currentSt, defaultSubjects[s], dates[d], st, "09:00 AM"));
            }
        }
    }
}

bool AttendanceTracker::loadFromFile() {
    std::ifstream inFile(dataFilePath.c_str());
    if (!inFile.is_open()) return false;

    records.clear();
    std::string line;
    while (std::getline(inFile, line)) {
        line = CommonUtils::trim(line);
        if (line.empty() || line[0] == '#') continue;
        AttendanceRecord r = AttendanceRecord::fromFileString(line);
        if (!r.getId().empty()) {
            records.push_back(r);
        }
    }
    inFile.close();
    return true;
}

bool AttendanceTracker::saveToFile() const {
    std::ofstream outFile(dataFilePath.c_str());
    if (!outFile.is_open()) return false;

    outFile << "# ID|StudentID|SubjectName|Date|Status|Timestamp\n";
    for (size_t i = 0; i < records.size(); ++i) {
        outFile << records[i].toFileString() << "\n";
    }
    outFile.close();
    return true;
}

bool AttendanceTracker::markAttendance(const std::string& studentId, const std::string& subjectName,
                                      const std::string& dateStr, const std::string& status) {
    holidayManager.validateDateForAttendance(dateStr);

    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].getStudentId() == studentId &&
            records[i].getSubjectName() == subjectName &&
            records[i].getDate() == dateStr) {
            records[i].setStatus(status);
            saveToFile();
            return true;
        }
    }

    std::string newId = "att_" + std::to_string(std::time(NULL) % 100000) + "_" + std::to_string(records.size() + 1);
    records.push_back(AttendanceRecord(newId, studentId, subjectName, dateStr, status, "Logged"));
    saveToFile();
    return true;
}

SubjectAttendanceStats AttendanceTracker::calculateStatsForSubject(const std::string& studentId, const std::string& subjectName) const {
    SubjectAttendanceStats stats;
    stats.subjectName = subjectName;
    stats.attendedCount = 0;
    stats.absentCount = 0;
    stats.cancelledCount = 0;

    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].getStudentId() == studentId && records[i].getSubjectName() == subjectName) {
            std::string st = records[i].getStatus();
            if (st == "PRESENT") stats.attendedCount++;
            else if (st == "ABSENT") stats.absentCount++;
            else if (st == "CANCELLED") stats.cancelledCount++;
        }
    }

    stats.totalHeld = stats.attendedCount + stats.absentCount;
    int P = stats.attendedCount;
    int T = stats.totalHeld;

    if (T == 0) {
        stats.percentage = 100.0;
        stats.safeMisses75 = 0;
        stats.requiredAttend75 = 0;
        stats.safeMisses65 = 0;
        stats.requiredAttend65 = 0;
        stats.statusTag = "Safe";
        return stats;
    }

    stats.percentage = (static_cast<double>(P) * 100.0) / static_cast<double>(T);

    // Target 75%
    if (stats.percentage >= 75.0) {
        stats.safeMisses75 = (100 * P - 75 * T) / 75;
        stats.requiredAttend75 = 0;
    } else {
        int numerator = 75 * T - 100 * P;
        stats.requiredAttend75 = (numerator + 24) / 25;
        stats.safeMisses75 = 0;
    }

    // Target 65% (Condonation)
    if (stats.percentage >= 65.0) {
        stats.safeMisses65 = (100 * P - 65 * T) / 65;
        stats.requiredAttend65 = 0;
    } else {
        int numerator = 65 * T - 100 * P;
        stats.requiredAttend65 = (numerator + 34) / 35;
        stats.safeMisses65 = 0;
    }

    if (stats.percentage >= 75.0) {
        stats.statusTag = "Safe";
    } else if (stats.percentage >= 65.0) {
        stats.statusTag = "Warning";
    } else {
        stats.statusTag = "Critical";
    }

    return stats;
}

std::vector<SubjectAttendanceStats> AttendanceTracker::getAllSubjectStats(const std::string& studentId) const {
    std::vector<std::string> subjects = getSubjectsForStudent(studentId);
    std::vector<SubjectAttendanceStats> list;
    for (size_t i = 0; i < subjects.size(); ++i) {
        list.push_back(calculateStatsForSubject(studentId, subjects[i]));
    }
    return list;
}

std::vector<AttendanceRecord> AttendanceTracker::getStudentRecords(const std::string& studentId) const {
    std::vector<AttendanceRecord> list;
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].getStudentId() == studentId) {
            list.push_back(records[i]);
        }
    }
    return list;
}

std::vector<AttendanceRecord> AttendanceTracker::getStudentRecordsForDate(const std::string& studentId, const std::string& dateStr) const {
    std::vector<AttendanceRecord> list;
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].getStudentId() == studentId && records[i].getDate() == dateStr) {
            list.push_back(records[i]);
        }
    }
    return list;
}

std::string AttendanceTracker::getStudentDashboardJson(const std::string& studentId) const {
    std::vector<SubjectAttendanceStats> stats = getAllSubjectStats(studentId);
    std::vector<AttendanceRecord> history = getStudentRecords(studentId);

    int totalAttended = 0;
    int totalHeld = 0;
    for (size_t i = 0; i < stats.size(); ++i) {
        totalAttended += stats[i].attendedCount;
        totalHeld += stats[i].totalHeld;
    }
    double overallPercentage = (totalHeld > 0) ? (static_cast<double>(totalAttended) * 100.0 / totalHeld) : 100.0;

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1);
    ss << "{"
       << "\"overallPercentage\":" << overallPercentage << ","
       << "\"totalAttended\":" << totalAttended << ","
       << "\"totalHeld\":" << totalHeld << ","
       << "\"subjectStats\":[";

    for (size_t i = 0; i < stats.size(); ++i) {
        if (i > 0) ss << ",";
        ss << stats[i].toJson();
    }
    ss << "],\"history\":[";

    for (size_t i = 0; i < history.size(); ++i) {
        if (i > 0) ss << ",";
        ss << history[i].toJson();
    }
    ss << "]}";

    return ss.str();
}
