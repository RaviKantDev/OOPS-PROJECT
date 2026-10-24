@echo off
title CampusFlow C++ Console CLI Demonstration Mode
color 0B

echo ======================================================================
echo       CAMPUSFLOW - 3RD SEM OOPS C++ INTERACTIVE CONSOLE DEMO
echo ======================================================================
echo.

if not exist bin (
    mkdir bin
)

echo [1/2] Compiling C++ Console Demo Application...
C:\MinGW\bin\g++.exe -std=c++11 -Wall src\CommonUtils.cpp src\Member1_UserAuth.cpp src\Member2_AssignmentHub.cpp src\Member3_AttendanceTracker.cpp src\main_cli.cpp -o bin\cli_demo.exe

if errorlevel 1 (
    echo.
    echo [ERROR] Compilation failed.
    pause
    exit /b 1
)

echo.
echo [2/2] Running Interactive Terminal Menu...
echo.
bin\cli_demo.exe
pause

