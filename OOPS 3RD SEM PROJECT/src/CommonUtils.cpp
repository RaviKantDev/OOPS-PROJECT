#include "../include/CommonUtils.h"
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cctype>

std::string CommonUtils::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string CommonUtils::getTodayDate() {
    std::time_t t = std::time(NULL);
    std::tm* now = std::localtime(&t);
    std::ostringstream oss;
    oss << (now->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << now->tm_mday;
    return oss.str();
}

bool CommonUtils::parseDate(const std::string& dateStr, int& year, int& month, int& day) {
    if (dateStr.length() < 10) return false;
    try {
        year = std::stoi(dateStr.substr(0, 4));
        month = std::stoi(dateStr.substr(5, 2));
        day = std::stoi(dateStr.substr(8, 2));
        if (month < 1 || month > 12 || day < 1 || day > 31) return false;
        return true;
    } catch (...) {
        return false;
    }
}

bool CommonUtils::isFutureDate(const std::string& dateStr) {
    std::string today = getTodayDate();
    // In "YYYY-MM-DD" format, simple lexicographical comparison is 100% accurate!
    return dateStr > today;
}

// Zeller's Congruence algorithm to calculate day of week
// Returns: 0 = Sunday, 1 = Monday, 2 = Tuesday, ..., 6 = Saturday
int CommonUtils::getDayOfWeek(int year, int month, int day) {
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    int k = year % 100;
    int j = year / 100;
    int h = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    int d = (h + 6) % 7; // Map 0=Sunday, 1=Monday, ..., 6=Saturday
    return d;
}

int CommonUtils::getDayOfWeek(const std::string& dateStr) {
    int y, m, d;
    if (!parseDate(dateStr, y, m, d)) return -1;
    return getDayOfWeek(y, m, d);
}

std::string CommonUtils::getDayName(const std::string& dateStr) {
    int dow = getDayOfWeek(dateStr);
    switch (dow) {
        case 0: return "Sunday";
        case 1: return "Monday";
        case 2: return "Tuesday";
        case 3: return "Wednesday";
        case 4: return "Thursday";
        case 5: return "Friday";
        case 6: return "Saturday";
        default: return "Unknown";
    }
}

bool CommonUtils::isWeekend(const std::string& dateStr) {
    int dow = getDayOfWeek(dateStr);
    return (dow == 0 || dow == 6); // 0 = Sunday, 6 = Saturday
}

std::string CommonUtils::escapeJson(const std::string& input) {
    std::ostringstream ss;
    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];
        if (c == '"') ss << "\\\"";
        else if (c == '\\') ss << "\\\\";
        else if (c == '\b') ss << "\\b";
        else if (c == '\f') ss << "\\f";
        else if (c == '\n') ss << "\\n";
        else if (c == '\r') ss << "\\r";
        else if (c == '\t') ss << "\\t";
        else ss << c;
    }
    return ss.str();
}

// Extract string value from JSON: e.g. "subject": "OOP C++"
std::string CommonUtils::extractJsonString(const std::string& json, const std::string& key) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = json.find(pattern);
    if (pos == std::string::npos) return "";

    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";

    // Find opening quote of value
    size_t start = json.find('"', pos);
    if (start == std::string::npos) return "";

    // Find closing quote, handling escaped quotes
    size_t end = start + 1;
    while (end < json.length()) {
        if (json[end] == '"' && json[end - 1] != '\\') break;
        end++;
    }
    if (end >= json.length()) return "";

    std::string val = json.substr(start + 1, end - start - 1);
    // Unescape common characters
    std::string result = "";
    for (size_t i = 0; i < val.length(); ++i) {
        if (val[i] == '\\' && i + 1 < val.length()) {
            if (val[i + 1] == '"') { result += '"'; i++; }
            else if (val[i + 1] == '\\') { result += '\\'; i++; }
            else if (val[i + 1] == 'n') { result += '\n'; i++; }
            else { result += val[i]; }
        } else {
            result += val[i];
        }
    }
    return result;
}

int CommonUtils::extractJsonInt(const std::string& json, const std::string& key, int defaultVal) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = json.find(pattern);
    if (pos == std::string::npos) return defaultVal;

    pos = json.find(':', pos);
    if (pos == std::string::npos) return defaultVal;

    size_t numStart = json.find_first_of("-0123456789", pos);
    if (numStart == std::string::npos) return defaultVal;

    size_t numEnd = json.find_first_not_of("-0123456789", numStart);
    std::string numStr = (numEnd == std::string::npos) ? json.substr(numStart) : json.substr(numStart, numEnd - numStart);
    try {
        return std::stoi(numStr);
    } catch (...) {
        return defaultVal;
    }
}

double CommonUtils::extractJsonDouble(const std::string& json, const std::string& key, double defaultVal) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = json.find(pattern);
    if (pos == std::string::npos) return defaultVal;

    pos = json.find(':', pos);
    if (pos == std::string::npos) return defaultVal;

    size_t numStart = json.find_first_of("-0123456789.", pos);
    if (numStart == std::string::npos) return defaultVal;

    size_t numEnd = json.find_first_not_of("-0123456789.", numStart);
    std::string numStr = (numEnd == std::string::npos) ? json.substr(numStart) : json.substr(numStart, numEnd - numStart);
    try {
        return std::stod(numStr);
    } catch (...) {
        return defaultVal;
    }
}

std::string CommonUtils::urlDecode(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && i + 2 < str.length()) {
            std::string hexStr = str.substr(i + 1, 2);
            char ch = static_cast<char>(std::strtol(hexStr.c_str(), NULL, 16));
            result += ch;
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

