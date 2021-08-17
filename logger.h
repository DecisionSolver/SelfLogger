//
// Created by scherkesov on 11.10.2020.
//

#ifndef GFB_LOGGER_H
#define GFB_LOGGER_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <vector>
#include <map>
#include <fstream>
#include <stdio.h>

#ifdef _WIN32

#define COLOR_TEXT_RED      12
#define COLOR_TEXT_GREEN    10
#define COLOR_TEXT_YELLOW   14
#define COLOR_TEXT_MAGENTA  13

#else

#define COLOR_TEXT_RED      31
#define COLOR_TEXT_GREEN    32
#define COLOR_TEXT_YELLOW   33
#define COLOR_TEXT_MAGENTA  35

#endif

enum LogLevel {
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO = 2,
    LOG_LEVEL_WARNING = 3,
    LOG_LEVEL_ERROR = 4,
    LOG_LEVEL_CRITICAL = 5,
    LOG_LEVEL_FATAL = 6,
};

class AbstractLoggerTarget {
public:
    explicit AbstractLoggerTarget(LogLevel level);

    bool applicable(LogLevel level);

    virtual void write(LogLevel level, const std::string &message) = 0;

	void enablePrint() { DisablePrint = false; }
	void disablePrint() { DisablePrint = true; }
protected:
    LogLevel mLevel;

	bool DisablePrint = false;
};

class ConsoleLoggerTarget : public AbstractLoggerTarget {
public:
    explicit ConsoleLoggerTarget(LogLevel level);

    void write(LogLevel level, const std::string &message) override;
};

class FileLoggerTarget : public AbstractLoggerTarget {
public:
    FileLoggerTarget(const std::string &filename, LogLevel level);

    void write(LogLevel level, const std::string &message) override;

private:
    std::ofstream mOut;
};

class MCVCLoggerTarget : public AbstractLoggerTarget {
public:
    MCVCLoggerTarget(LogLevel level);

    void write(LogLevel level, const std::string &message) override;
};

class CppLogger {
public:
    static void registerTarget(AbstractLoggerTarget *t);

    static void print(char const *function, char const *file, long line,
                      LogLevel level, const std::string &message);
	static void DisablePrintAll();
	static void EnablePrintAll();
private:
    static std::vector<AbstractLoggerTarget *> mTargets;

    static std::string getDateTime();
};

#define MacroStr(x) #x
#define MacroStr2(x) MacroStr(x)
#define ToDo(desc) __pragma(message(__FILE__ "(" MacroStr2(__LINE__) "): " #desc))

#ifdef __GNUC__

#define Logger_Debug(message)    CppLogger::print(__PRETTY_FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_DEBUG),    (message));
#define Logger_Info(message)     CppLogger::print(__PRETTY_FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_INFO),     (message));
#define Logger_Warn(message)     CppLogger::print(__PRETTY_FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_WARNING),  (message));
#define Logger_Error(message)    CppLogger::print(__PRETTY_FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_ERROR),    (message));
#define Logger_Critical(message) CppLogger::print(__PRETTY_FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_CRITICAL), (message));
#define Logger_Fatal(message)    CppLogger::print(__PRETTY_FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_FATAL),    (message));

#else

#define Logger_Debug(message)    CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_DEBUG),    (message));
#define Logger_Info(message)     CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_INFO),     (message));
#define Logger_Warn(message)     CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_WARNING),  (message));
#define Logger_Error(message)    CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_ERROR),    (message));
#define Logger_Critical(message) CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_CRITICAL), (message));
#define Logger_Fatal(message)    CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_FATAL),    (message));

ToDo("Bug: Overflow These Functions If ... String More Than 2048 Bits")
#define Logger_Debug_F(message, ...) {                                              \
    char buff[2048];																\
    sprintf_s(buff, sizeof buff, message, __VA_ARGS__);                             \
    CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_DEBUG), buff);    \
}
#define Logger_Info_F(message, ...) {                                               \
    char buff[2048];																\
    sprintf_s(buff, sizeof buff, message, __VA_ARGS__);                             \
    CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_INFO), buff);     \
}
#define Logger_Warn_F(message, ...) {                                               \
    char buff[2048];																\
    sprintf_s(buff, sizeof buff, message, __VA_ARGS__);                             \
    CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_WARNING), buff);  \
}
#define Logger_Error_F(message, ...) {                                              \
    char buff[2048];																\
    sprintf_s(buff, sizeof buff, message, __VA_ARGS__);                             \
    CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_ERROR), buff);    \
}
#define Logger_Critical_F(message, ...) {                                           \
    char buff[2048];																\
    sprintf_s(buff, sizeof buff, message, __VA_ARGS__);                             \
    CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_CRITICAL), buff); \
}
#define Logger_Fatal_F(message, ...) {                                              \
    char buff[2048];																\
    sprintf_s(buff, sizeof buff, message, __VA_ARGS__);                             \
    CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_FATAL), buff);    \
}

#endif

#endif //GFB_LOGGER_H
