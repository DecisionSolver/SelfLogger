//
// Created by GOFORBROKE on 23.10.2020.
//

#include "logger.h"

std::map<LogLevel, int> LogLevelColor = {
        {LOG_LEVEL_DEBUG, 0},
        {LOG_LEVEL_INFO,     COLOR_TEXT_GREEN},
        {LOG_LEVEL_WARNING,  COLOR_TEXT_YELLOW},
        {LOG_LEVEL_ERROR,    COLOR_TEXT_MAGENTA},
        {LOG_LEVEL_CRITICAL, COLOR_TEXT_RED},
        {LOG_LEVEL_FATAL,    COLOR_TEXT_RED},
};

std::map<LogLevel, std::string> LogLevelLabel = {
        {LOG_LEVEL_DEBUG,    "DEBUG"},
        {LOG_LEVEL_INFO,     "INFO"},
        {LOG_LEVEL_WARNING,  "WARNING"},
        {LOG_LEVEL_ERROR,    "ERROR"},
        {LOG_LEVEL_CRITICAL, "CRITICAL"},
        {LOG_LEVEL_FATAL,    "FATAL"},
};


AbstractLoggerTarget::AbstractLoggerTarget(LogLevel level) : mLevel(level) {}

bool AbstractLoggerTarget::applicable(LogLevel level) {
    return level >= mLevel;
}

ConsoleLoggerTarget::ConsoleLoggerTarget(LogLevel level)
        : AbstractLoggerTarget(level) {
}

void ConsoleLoggerTarget::write(LogLevel level, const std::string &message) {
    int colorCode = LogLevelColor[level];

    if (level >= LOG_LEVEL_ERROR) {
        std::cerr << message << std::endl;
    } else if (level == LOG_LEVEL_INFO || level == LOG_LEVEL_WARNING) {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colorCode);
        std::cout << message << std::endl;
#else
        std::cout
                << "\033[" << colorCode << "m"
                << message
                << "\033[0m"
                << std::endl;
#endif
    } else if (level == LOG_LEVEL_DEBUG) {
#if defined(DEBUG) || defined(_DEBUG)
        std::cout << message << std::endl;
#endif
    }
}

FileLoggerTarget::FileLoggerTarget(const std::string &filename, LogLevel level)
        : AbstractLoggerTarget(level), mOut(filename, std::ofstream::app) {}

void FileLoggerTarget::write(LogLevel level, const std::string &message) {
    mOut << message << std::endl;
}

MCVCLoggerTarget::MCVCLoggerTarget(LogLevel level) : AbstractLoggerTarget(level) {

}

void MCVCLoggerTarget::write(LogLevel level, const std::string &message) {
#ifdef _MSC_VER
    OutputDebugStringA(message.c_str());
    OutputDebugStringA("\n");
#endif
}


std::vector<AbstractLoggerTarget *> CppLogger::mTargets = {
        new ConsoleLoggerTarget(LOG_LEVEL_DEBUG),
#ifdef _MSC_VER
        new MCVCLoggerTarget(LOG_LEVEL_DEBUG),
#endif
};

void CppLogger::registerTarget(AbstractLoggerTarget *t) {
    mTargets.push_back(t);
}

void CppLogger::print(
        const char *function, const char *file, long line,
        LogLevel level, const std::string &message
) {

    std::string levelLabel = LogLevelLabel[level];
    std::string text = getDateTime() + " [" + levelLabel + "] - "
                       + message + "\t" + file + ":" + std::to_string(line) + " " + function;

    for (auto &t : mTargets) {
        if (!t->applicable(level))
            continue;

        t->write(level, text);
    }

    if (level == LOG_LEVEL_CRITICAL) throw std::runtime_error(message);
    if (level == LOG_LEVEL_FATAL) exit(EXIT_FAILURE);

//#ifdef _WIN32
//
//#if defined(_DEBUG)
//        std::string text =
//                "[" + levelLabel + "] - " + getDateTime() + " - "
//                + message + " at " + file + ":" + std::to_string(line) + " " + function;
//        OutputDebugStringA(text.c_str());
//#else
//        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//        SetConsoleTextAttribute(hConsole, colorCode);
//        std::cout
//                << "[" << levelLabel << "] - " << message;
//
//        SetConsoleTextAttribute(hConsole, 0);
//        std::cout
//                << " at " << file << ":" << line << " " << function
//                << std::endl;
//#endif
//
//#else
//        std::cout
//                << "\033[" << colorCode << "m"
//                << "[" << levelLabel << "] - " << getDateTime() << " - " << message
//                << "\033[0m"
//                << " at " << file << ":" << line << " " << function
//                << std::endl;
//#endif
}

std::string CppLogger::getDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(now);
    auto buf = std::localtime(&start_time);
    char timedisplay[100];
    size_t len = std::strftime(timedisplay, sizeof(timedisplay), "%Y-%m-%d %H:%M:%S", buf);
//        size_t len = std::strftime(timedisplay, sizeof(timedisplay), "%H:%M:%S", buf);
    return std::string(timedisplay, len);
}
