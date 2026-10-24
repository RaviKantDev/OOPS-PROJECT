#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>

// ============================================================================
// OOP CONCEPT: Exception Hierarchy (Inheritance in Exceptions)
// Base exception for the College Portal
// ============================================================================
class AppException : public std::exception {
protected:
    std::string message;
public:
    explicit AppException(const std::string& msg) : message(msg) {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class FutureDateException : public AppException {
public:
    explicit FutureDateException(const std::string& date) 
        : AppException("Cannot mark attendance on future date: " + date) {}
};

class WeekendException : public AppException {
public:
    explicit WeekendException(const std::string& date, const std::string& dayName) 
        : AppException(date + " is " + dayName + " (Weekend). Attendance cannot be marked.") {}
};

class HolidayException : public AppException {
public:
    explicit HolidayException(const std::string& date, const std::string& reason) 
        : AppException(date + " is an official college holiday: " + reason) {}
};

class UnauthorizedException : public AppException {
public:
    explicit UnauthorizedException(const std::string& msg = "Permission denied: Action reserved for Class Representative (CR).") 
        : AppException(msg) {}
};

// ============================================================================
// Utility Helper Functions for Date, String & JSON Operations
// ============================================================================
class CommonUtils {
public:
    // String trimming
    static std::string trim(const std::string& s);

    // Current system date formatted as "YYYY-MM-DD"
    static std::string getTodayDate();

    // Date validation
    static bool parseDate(const std::string& dateStr, int& year, int& month, int& day);
    static bool isFutureDate(const std::string& dateStr);
    
    // Day of week: 0 = Sunday, 1 = Monday, ..., 6 = Saturday (Zeller's Congruence)
    static int getDayOfWeek(int year, int month, int day);
    static int getDayOfWeek(const std::string& dateStr);
    static std::string getDayName(const std::string& dateStr);
    static bool isWeekend(const std::string& dateStr);

    // Beginner-friendly JSON helper utilities
    static std::string escapeJson(const std::string& input);
    static std::string extractJsonString(const std::string& json, const std::string& key);
    static int extractJsonInt(const std::string& json, const std::string& key, int defaultVal = 0);
    static double extractJsonDouble(const std::string& json, const std::string& key, double defaultVal = 0.0);

    // URL / query parameter decoder
    static std::string urlDecode(const std::string& str);
};

#endif // COMMON_UTILS_H

