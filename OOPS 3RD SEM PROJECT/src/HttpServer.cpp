#include "../include/HttpServer.h"
#include "../include/CommonUtils.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

// ============================================================================
// HttpRequest & HttpResponse
// ============================================================================
std::string HttpRequest::getQueryParam(const std::string& key) const {
    std::string search = key + "=";
    size_t pos = query.find(search);
    if (pos == std::string::npos) return "";

    size_t start = pos + search.length();
    size_t end = query.find('&', start);
    std::string val = (end == std::string::npos) ? query.substr(start) : query.substr(start, end - start);
    return CommonUtils::urlDecode(val);
}

std::string HttpResponse::toString() const {
    std::ostringstream ss;
    ss << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
    ss << "Content-Type: " << contentType << "\r\n";
    ss << "Content-Length: " << body.length() << "\r\n";
    ss << "Access-Control-Allow-Origin: *\r\n";
    ss << "Access-Control-Allow-Methods: GET, POST, OPTIONS, DELETE\r\n";
    ss << "Access-Control-Allow-Headers: Content-Type, Authorization\r\n";
    ss << "Connection: close\r\n\r\n";
    ss << body;
    return ss.str();
}

// ============================================================================
// CollegeWebServer Implementation
// ============================================================================
CollegeWebServer::CollegeWebServer(int port, UserManager& um, AssignmentManager& am, AttendanceTracker& at)
    : port(port), userManager(um), assignmentManager(am), attendanceTracker(at) {
    standardSubjects.push_back("Object Oriented Programming (C++)");
    standardSubjects.push_back("Data Structures & Algorithms");
    standardSubjects.push_back("Computer Organization & Architecture");
    standardSubjects.push_back("Discrete Mathematics & Graph Theory");
    standardSubjects.push_back("Digital Logic & Design");
}

std::string CollegeWebServer::readFileContent(const std::string& path) const {
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

HttpRequest CollegeWebServer::parseRequest(const std::string& raw) const {
    HttpRequest req;
    std::istringstream stream(raw);
    std::string requestLine;
    if (std::getline(stream, requestLine)) {
        std::istringstream lineStream(requestLine);
        lineStream >> req.method;
        std::string fullPath;
        lineStream >> fullPath;

        size_t qPos = fullPath.find('?');
        if (qPos != std::string::npos) {
            req.path = fullPath.substr(0, qPos);
            req.query = fullPath.substr(qPos + 1);
        } else {
            req.path = fullPath;
            req.query = "";
        }
    }

    std::string headerLine;
    while (std::getline(stream, headerLine)) {
        headerLine = CommonUtils::trim(headerLine);
        if (headerLine.empty()) break;
        size_t col = headerLine.find(':');
        if (col != std::string::npos) {
            std::string key = headerLine.substr(0, col);
            std::string val = headerLine.substr(col + 1);
            req.headers[CommonUtils::trim(key)] = CommonUtils::trim(val);
        }
    }

    // Body
    std::ostringstream bodyStream;
    bodyStream << stream.rdbuf();
    req.body = bodyStream.str();

    return req;
}

HttpResponse CollegeWebServer::handleRequest(const HttpRequest& req) {
    if (req.method == "OPTIONS") {
        return HttpResponse(200, "text/plain", "OK");
    }

    // Static Web Assets
    if (req.path == "/" || req.path == "/index.html") {
        std::string html = readFileContent("web/index.html");
        if (html.empty()) return HttpResponse(404, "text/plain", "index.html not found");
        return HttpResponse(200, "text/html; charset=utf-8", html);
    }
    if (req.path == "/style.css") {
        std::string css = readFileContent("web/style.css");
        if (css.empty()) return HttpResponse(404, "text/plain", "style.css not found");
        return HttpResponse(200, "text/css; charset=utf-8", css);
    }
    if (req.path == "/app.js") {
        std::string js = readFileContent("web/app.js");
        if (js.empty()) return HttpResponse(404, "text/plain", "app.js not found");
        return HttpResponse(200, "application/javascript; charset=utf-8", js);
    }

    // API ENDPOINTS

    // 1. Meta information (B.Tech IT-B 3rd Sem)
    if (req.path == "/api/meta" && req.method == "GET") {
        std::ostringstream ss;
        ss << "{"
           << "\"todayDate\":\"" << CommonUtils::getTodayDate() << "\","
           << "\"todayDay\":\"" << CommonUtils::getDayName(CommonUtils::getTodayDate()) << "\","
           << "\"isWeekend\":" << (CommonUtils::isWeekend(CommonUtils::getTodayDate()) ? "true" : "false") << ","
           << "\"course\":\"B.Tech\","
           << "\"branch\":\"Information Technology\","
           << "\"section\":\"IT-B\","
           << "\"semester\":3,"
           << "\"crName\":\"Priyanshu Raj\","
           << "\"subjects\":[";
        for (size_t i = 0; i < standardSubjects.size(); ++i) {
            if (i > 0) ss << ",";
            ss << "\"" << CommonUtils::escapeJson(standardSubjects[i]) << "\"";
        }
        ss << "]}";
        return HttpResponse(200, "application/json", ss.str());
    }

    // 2. Member 1: User Login
    if (req.path == "/api/login" && req.method == "POST") {
        std::string u = CommonUtils::extractJsonString(req.body, "username");
        std::string p = CommonUtils::extractJsonString(req.body, "password");
        User* user = userManager.authenticate(u, p);
        if (user != NULL) {
            std::ostringstream ss;
            ss << "{\"success\":true,\"user\":" << user->toJson() << "}";
            return HttpResponse(200, "application/json", ss.str());
        } else {
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"Invalid username or password\"}");
        }
    }

    // 3. Member 1: Profile Update (Edit roll, enrollment, semester, etc.)
    if (req.path == "/api/profile/update" && req.method == "POST") {
        std::string userId = CommonUtils::extractJsonString(req.body, "userId");
        std::string fullName = CommonUtils::extractJsonString(req.body, "fullName");
        std::string rollNumber = CommonUtils::extractJsonString(req.body, "rollNumber");
        std::string enrollmentNumber = CommonUtils::extractJsonString(req.body, "enrollmentNumber");
        int semester = CommonUtils::extractJsonInt(req.body, "semester", 3);
        std::string branch = CommonUtils::extractJsonString(req.body, "branch");
        std::string section = CommonUtils::extractJsonString(req.body, "section");

        if (userManager.updateProfile(userId, fullName, rollNumber, enrollmentNumber, semester, branch, section)) {
            User* updated = userManager.findById(userId);
            std::ostringstream ss;
            ss << "{\"success\":true,\"message\":\"Profile updated successfully\",\"user\":" << updated->toJson() << "}";
            return HttpResponse(200, "application/json", ss.str());
        } else {
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"Failed to update profile\"}");
        }
    }

    // 4. Member 1: Register New Student
    if (req.path == "/api/register" && req.method == "POST") {
        std::string u = CommonUtils::extractJsonString(req.body, "username");
        std::string p = CommonUtils::extractJsonString(req.body, "password");
        std::string name = CommonUtils::extractJsonString(req.body, "fullName");
        std::string roll = CommonUtils::extractJsonString(req.body, "rollNumber");
        std::string enr = CommonUtils::extractJsonString(req.body, "enrollmentNumber");
        int sem = CommonUtils::extractJsonInt(req.body, "semester", 3);
        std::string br = CommonUtils::extractJsonString(req.body, "branch");
        std::string sec = CommonUtils::extractJsonString(req.body, "section");
        if (br.empty()) br = "Information Technology";
        if (sec.empty()) sec = "IT-B";
        if (name.empty()) name = u;
        if (roll.empty()) {
            if (enr.length() >= 2) roll = "23ITB" + enr.substr(enr.length() - 2);
            else roll = "23ITB00";
        }

        if (u.empty() || p.empty() || enr.empty()) {
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"Username, password and enrollment number are required\"}");
        }

        if (userManager.registerStudent(u, p, name, roll, enr, sem, br, sec)) {
            User* newUser = userManager.findByUsername(u);
            std::ostringstream ss;
            ss << "{\"success\":true,\"message\":\"Account registered successfully\",\"user\":" << newUser->toJson() << "}";
            return HttpResponse(200, "application/json", ss.str());
        } else {
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"Username already exists\"}");
        }
    }

    // 5. Member 3: Get Subjects for specific student (includes custom subjects)
    if (req.path == "/api/subjects" && req.method == "GET") {
        std::string studentId = req.getQueryParam("studentId");
        if (studentId.empty()) studentId = "u_st01";

        std::vector<std::string> subjects = attendanceTracker.getSubjectsForStudent(studentId);
        std::ostringstream ss;
        ss << "[";
        for (size_t i = 0; i < subjects.size(); ++i) {
            if (i > 0) ss << ",";
            ss << "\"" << CommonUtils::escapeJson(subjects[i]) << "\"";
        }
        ss << "]";
        return HttpResponse(200, "application/json", ss.str());
    }

    // 6. Member 3: Add Custom Subject for student
    if (req.path == "/api/subjects/add" && req.method == "POST") {
        std::string studentId = CommonUtils::extractJsonString(req.body, "studentId");
        std::string subjectName = CommonUtils::extractJsonString(req.body, "subjectName");

        if (studentId.empty() || subjectName.empty()) {
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"Student ID and Subject Name are required\"}");
        }

        if (attendanceTracker.addCustomSubject(studentId, subjectName)) {
            return HttpResponse(200, "application/json", "{\"success\":true,\"message\":\"Subject added successfully to your curriculum\"}");
        } else {
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"Subject already enrolled or invalid\"}");
        }
    }

    // 7. Member 2: Get all assignments
    if (req.path == "/api/assignments" && req.method == "GET") {
        return HttpResponse(200, "application/json", assignmentManager.getAllAssignmentsJson());
    }

    // 8. Member 2: Add Assignment (CR action)
    if (req.path == "/api/assignments" && req.method == "POST") {
        std::string code = CommonUtils::extractJsonString(req.body, "subjectCode");
        std::string subject = CommonUtils::extractJsonString(req.body, "subjectName");
        std::string title = CommonUtils::extractJsonString(req.body, "title");
        std::string desc = CommonUtils::extractJsonString(req.body, "description");
        std::string assigned = CommonUtils::extractJsonString(req.body, "assignedDate");
        std::string deadline = CommonUtils::extractJsonString(req.body, "deadlineDate");
        std::string medium = CommonUtils::extractJsonString(req.body, "submissionMedium");
        std::string faculty = CommonUtils::extractJsonString(req.body, "facultyName");
        int marks = CommonUtils::extractJsonInt(req.body, "maxMarks", 20);
        std::string inst = CommonUtils::extractJsonString(req.body, "instructions");

        if (assigned.empty()) assigned = CommonUtils::getTodayDate();
        if (subject.empty() || title.empty() || deadline.empty() || medium.empty()) {
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"Subject, Title, Deadline, and Submission Medium are required\"}");
        }

        Assignment newAsgn("", code, subject, title, desc, assigned, deadline, medium, faculty, marks, inst);
        assignmentManager.addAssignment(newAsgn);
        return HttpResponse(200, "application/json", "{\"success\":true,\"message\":\"Assignment created successfully\"}");
    }

    // 9. Member 2: Delete Assignment
    if ((req.path == "/api/assignments/delete" && req.method == "POST") ||
        (req.path == "/api/assignments" && req.method == "DELETE")) {
        std::string id = CommonUtils::extractJsonString(req.body, "id");
        if (id.empty()) id = req.getQueryParam("id");

        if (assignmentManager.deleteAssignment(id)) {
            return HttpResponse(200, "application/json", "{\"success\":true,\"message\":\"Assignment deleted successfully\"}");
        } else {
            return HttpResponse(404, "application/json", "{\"success\":false,\"error\":\"Assignment not found\"}");
        }
    }

    // 10. Member 3: Get Holidays
    if (req.path == "/api/holidays" && req.method == "GET") {
        return HttpResponse(200, "application/json", attendanceTracker.getHolidayManager().getAllHolidaysJson());
    }

    // 11. Member 3: Add Holiday (CR action)
    if (req.path == "/api/holidays" && req.method == "POST") {
        std::string date = CommonUtils::extractJsonString(req.body, "date");
        std::string desc = CommonUtils::extractJsonString(req.body, "description");
        std::string by = CommonUtils::extractJsonString(req.body, "declaredBy");
        if (by.empty()) by = "Priyanshu Raj (CR)";

        if (date.empty() || desc.empty()) {
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"Date and description are required\"}");
        }

        if (attendanceTracker.getHolidayManager().addHoliday(date, desc, by)) {
            return HttpResponse(200, "application/json", "{\"success\":true,\"message\":\"Holiday declared successfully\"}");
        } else {
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"Holiday already exists on this date\"}");
        }
    }

    // 12. Member 3: Delete Holiday
    if (req.path == "/api/holidays/delete" && req.method == "POST") {
        std::string date = CommonUtils::extractJsonString(req.body, "date");
        if (attendanceTracker.getHolidayManager().removeHoliday(date)) {
            return HttpResponse(200, "application/json", "{\"success\":true,\"message\":\"Holiday removed successfully\"}");
        } else {
            return HttpResponse(404, "application/json", "{\"success\":false,\"error\":\"Holiday not found\"}");
        }
    }

    // 13. Member 3: Get Attendance Analytics for student (including custom subjects)
    if (req.path == "/api/attendance" && req.method == "GET") {
        std::string studentId = req.getQueryParam("studentId");
        if (studentId.empty()) studentId = "u_st01";

        std::string json = attendanceTracker.getStudentDashboardJson(studentId);
        return HttpResponse(200, "application/json", json);
    }

    // 14. Member 3: Mark Attendance (Strict date & holiday validation)
    if (req.path == "/api/attendance" && req.method == "POST") {
        std::string studentId = CommonUtils::extractJsonString(req.body, "studentId");
        std::string subject = CommonUtils::extractJsonString(req.body, "subjectName");
        std::string date = CommonUtils::extractJsonString(req.body, "date");
        std::string status = CommonUtils::extractJsonString(req.body, "status");

        if (studentId.empty()) studentId = "u_st01";
        if (date.empty()) date = CommonUtils::getTodayDate();
        if (status.empty()) status = "PRESENT";

        // TRY-CATCH EXCEPTION HANDLING DEMONSTRATION
        try {
            attendanceTracker.markAttendance(studentId, subject, date, status);
            return HttpResponse(200, "application/json", "{\"success\":true,\"message\":\"Attendance marked successfully\"}");
        } catch (const FutureDateException& e) {
            std::string err = e.what();
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"" + CommonUtils::escapeJson(err) + "\"}");
        } catch (const WeekendException& e) {
            std::string err = e.what();
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"" + CommonUtils::escapeJson(err) + "\"}");
        } catch (const HolidayException& e) {
            std::string err = e.what();
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"" + CommonUtils::escapeJson(err) + "\"}");
        } catch (const std::exception& e) {
            std::string err = e.what();
            return HttpResponse(400, "application/json", "{\"success\":false,\"error\":\"" + CommonUtils::escapeJson(err) + "\"}");
        }
    }

    return HttpResponse(404, "application/json", "{\"error\":\"Route not found\"}");
}

void CollegeWebServer::start() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed with error code: " << WSAGetLastError() << "\n";
        return;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "\n======================================================================\n";
    std::cout << "      NATIVE C++ WEB SERVER STARTED SUCCESSFULLY ON PORT " << port << "     \n";
    std::cout << "      Course: B.Tech IT-B (3rd Semester) Academic Portal              \n";
    std::cout << "======================================================================\n";
    std::cout << " Open in Browser: http://localhost:" << port << "\n";
    std::cout << " Press Ctrl+C in this terminal window to stop the server.\n";
    std::cout << "======================================================================\n\n";

    while (true) {
        sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);

        if (clientSocket == INVALID_SOCKET) {
            continue;
        }

        std::string rawRequest = "";
        char buffer[4096];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0) {
            rawRequest.append(buffer, bytesRead);
        }

        // Check if there is Content-Length and read complete body
        size_t headerEndPos = rawRequest.find("\r\n\r\n");
        if (headerEndPos == std::string::npos) {
            headerEndPos = rawRequest.find("\n\n");
        }

        if (headerEndPos != std::string::npos) {
            size_t clPos = rawRequest.find("Content-Length:");
            if (clPos == std::string::npos) {
                clPos = rawRequest.find("content-length:");
            }
            if (clPos != std::string::npos) {
                size_t valStart = rawRequest.find_first_of("0123456789", clPos);
                size_t valEnd = rawRequest.find_first_not_of("0123456789", valStart);
                int contentLen = std::stoi(rawRequest.substr(valStart, valEnd - valStart));
                
                size_t bodyStart = (rawRequest.find("\r\n\r\n") != std::string::npos) ? (rawRequest.find("\r\n\r\n") + 4) : (rawRequest.find("\n\n") + 2);
                int currentBodyLen = rawRequest.length() - bodyStart;
                
                while (currentBodyLen < contentLen) {
                    int more = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                    if (more <= 0) break;
                    rawRequest.append(buffer, more);
                    currentBodyLen += more;
                }
            }
        }

        if (!rawRequest.empty()) {
            HttpRequest req = parseRequest(rawRequest);
            HttpResponse resp = handleRequest(req);
            std::string responseStr = resp.toString();

            send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
        }

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
}
