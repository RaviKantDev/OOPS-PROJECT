#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>
#include <map>
#include "Member1_UserAuth.h"
#include "Member2_AssignmentHub.h"
#include "Member3_AttendanceTracker.h"

// ============================================================================
// LIGHTWEIGHT NATIVE C++ HTTP WEB SERVER (WINSOCK2)
// Serves the soothing minimalistic web UI and REST API endpoints.
// Connects Member 1, Member 2, and Member 3 OOPS components to the browser.
// ============================================================================

class HttpRequest {
public:
    std::string method;
    std::string path;
    std::string query;
    std::map<std::string, std::string> headers;
    std::string body;

    std::string getQueryParam(const std::string& key) const;
};

class HttpResponse {
public:
    int statusCode;
    std::string statusText;
    std::string contentType;
    std::string body;

    HttpResponse(int code = 200, const std::string& type = "application/json", const std::string& content = "")
        : statusCode(code), statusText("OK"), contentType(type), body(content) {
        if (code == 400) statusText = "Bad Request";
        else if (code == 403) statusText = "Forbidden";
        else if (code == 404) statusText = "Not Found";
        else if (code == 500) statusText = "Internal Server Error";
    }

    std::string toString() const;
};

class CollegeWebServer {
private:
    int port;
    UserManager& userManager;
    AssignmentManager& assignmentManager;
    AttendanceTracker& attendanceTracker;
    std::vector<std::string> standardSubjects;

    HttpRequest parseRequest(const std::string& raw) const;
    HttpResponse handleRequest(const HttpRequest& req);
    std::string readFileContent(const std::string& path) const;

public:
    CollegeWebServer(int port, UserManager& um, AssignmentManager& am, AttendanceTracker& at);
    void start();
};

#endif // HTTP_SERVER_H

