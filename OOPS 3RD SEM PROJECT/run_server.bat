@echo off
title CampusFlow C++ Academic & Attendance Web Server
color 0A

echo ======================================================================
echo          CAMPUSFLOW - 3RD SEM OOPS C++ WEB BACKEND LAUNCHER
echo ======================================================================
echo.

if not exist bin (
    mkdir bin
)

echo [1/2] Compiling C++ Web Server with MinGW g++...
C:\MinGW\bin\g++.exe -std=c++11 -Wall src\CommonUtils.cpp src\Member1_UserAuth.cpp src\Member2_AssignmentHub.cpp src\Member3_AttendanceTracker.cpp src\HttpServer.cpp src\main_server.cpp -lws2_32 -o bin\server.exe

if errorlevel 1 (
    echo.
    echo [ERROR] Compilation failed. Please check compiler path.
    pause
    exit /b 1
)

echo.
echo [2/2] Launching C++ Web Server on http://localhost:8080 ...
echo Opening your default browser...
start http://localhost:8080

echo.
echo ======================================================================
echo  Server is running! Keep this terminal open while using the website.
echo  Press Ctrl+C to stop the server when done.
echo ======================================================================
echo.
bin\server.exe
pause

