#pragma once
#include <iostream>
#include <Windows.h>

#if _DEBUG == 1

#define CMD_COLOR_RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
#define CMD_COLOR_YELLOW SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
#define CMD_COLOR_GREEN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
#define CMD_COLOR_WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);

#define LOG_ERROR(x)   CMD_COLOR_RED;    std::cout<<("[ERROR]")<<"::"<<x<<'\n';   CMD_COLOR_WHITE;
#define LOG_WARNING(x) CMD_COLOR_YELLOW; std::cout<<("[WARNING]")<<"::"<<x<<'\n'; CMD_COLOR_WHITE;
#define LOG(x)		   CMD_COLOR_GREEN;  std::cout<<("[LOG]")<<"::"<<x<<'\n';     CMD_COLOR_WHITE;

#define ASSERT(x) if(!(x)) __debugbreak()

#else
#define LOG_ERROR(x)
#define LOG_WARNING(x)
#define LOG(x)
#define ASSERT(x) if(!(x)) __debugbreak()
#endif