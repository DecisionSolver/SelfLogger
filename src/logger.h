//
// Created by scherkesov on 11.10.2020.
//

#ifndef GFB_LOGGER_H
#define GFB_LOGGER_H

#include "../pch.h"

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

#define Logger_Debug_F(message, ...) CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_DEBUG), fmt::format(message, __VA_ARGS__).c_str());

#define Logger_Info_F(message, ...) CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_INFO), fmt::format(message, __VA_ARGS__).c_str());

#define Logger_Warn_F(message, ...) CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_WARNING), fmt::format(message, __VA_ARGS__).c_str());

#define Logger_Error_F(message, ...) CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_ERROR), fmt::format(message, __VA_ARGS__).c_str());

#define Logger_Critical_F(message, ...) CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_CRITICAL), fmt::format(message, __VA_ARGS__).c_str());

#define Logger_Fatal_F(message, ...) CppLogger::print(__FUNCTION__, __FILE__, __LINE__, (LOG_LEVEL_FATAL), fmt::format(message, __VA_ARGS__).c_str());

#endif

#endif //GFB_LOGGER_H
