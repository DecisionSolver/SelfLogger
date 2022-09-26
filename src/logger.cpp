//
// Created by GOFORBROKE on 23.10.2020.
//
#include "pch.h"
#include "logger.h"
#include <mutex>
#include <Windows.h>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <map>
#include <stdio.h>

std::map<LogLevel, int> LogLevelColor = {
	{ LOG_LEVEL_DEBUG, 0 },
	{ LOG_LEVEL_INFO, COLOR_TEXT_GREEN },
	{ LOG_LEVEL_WARNING, COLOR_TEXT_YELLOW },
	{ LOG_LEVEL_ERROR, COLOR_TEXT_MAGENTA },
	{ LOG_LEVEL_CRITICAL, COLOR_TEXT_RED },
	{ LOG_LEVEL_FATAL, COLOR_TEXT_RED },
};

std::mutex m_mutex;
std::map<LogLevel, std::string> LogLevelLabel = {
	{ LOG_LEVEL_DEBUG, "DEBUG" },
	{ LOG_LEVEL_INFO, "INFO" },
	{ LOG_LEVEL_WARNING, "WARNING" },
	{ LOG_LEVEL_ERROR, "ERROR" },
	{ LOG_LEVEL_CRITICAL, "CRITICAL" },
	{ LOG_LEVEL_FATAL, "FATAL" },
};

AbstractLoggerTarget::AbstractLoggerTarget(LogLevel level) : mLevel(level) {}

bool AbstractLoggerTarget::applicable(LogLevel level) {
	return level >= mLevel;
}

ConsoleLoggerTarget::ConsoleLoggerTarget(LogLevel level)
		: AbstractLoggerTarget(level) {
}

void ConsoleLoggerTarget::write(LogLevel level, const std::string &message) {
	if (DisablePrint || LogLevelLabel.empty() || LogLevelColor.empty()) return;

	int colorCode = LogLevelColor[level];

	if (level >= LOG_LEVEL_ERROR) {
		std::cerr << message << std::endl;
	} else if (level == LOG_LEVEL_INFO || level == LOG_LEVEL_WARNING) {
#ifdef _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (WORD)colorCode);
		std::cout << message << std::endl;
										/* 7-White */
		SetConsoleTextAttribute(hConsole, 7);
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
	if (DisablePrint) return;
	mOut << message << std::endl;
	mOut.flush();
}

MCVCLoggerTarget::MCVCLoggerTarget(LogLevel level) : AbstractLoggerTarget(level) {

}

void MCVCLoggerTarget::write(LogLevel level, const std::string &message) {
	if (DisablePrint) return;
#if defined(_MSC_VER)
	OutputDebugStringA(message.c_str());
	OutputDebugStringA("\n");
#endif
}


std::vector<std::shared_ptr<AbstractLoggerTarget>> CppLogger::mTargets = {
	std::make_shared<ConsoleLoggerTarget>(LOG_LEVEL_DEBUG),
#if defined(_MSC_VER)
	std::make_shared<MCVCLoggerTarget>(LOG_LEVEL_DEBUG),
#endif
};

void CppLogger::registerTarget(const std::shared_ptr<AbstractLoggerTarget> &t) {
	mTargets.push_back(t);
}

void CppLogger::print(
	const char *function, const char *file, long line,
	LogLevel level, const std::string &message
) {
	std::scoped_lock<std::mutex> lock(m_mutex);
	if (LogLevelLabel.empty() || LogLevelColor.empty()) return;
	int colorCode = LogLevelColor[level];

	std::string levelLabel = LogLevelLabel[level];
	std::string text = getDateTime() + " [" + levelLabel + "] - "
					   + message + "\t" + file + ":" + std::to_string(line) + " " + function + "\n";

	for (auto &t : mTargets) {
		if (!t->applicable(level))
			continue;

		t->write(level, text);
	}

	if (level == LOG_LEVEL_CRITICAL) throw std::runtime_error(message);
	if (level == LOG_LEVEL_FATAL) exit(EXIT_FAILURE);

#ifdef _WIN32

#if defined(_DEBUG)
		OutputDebugStringA(text.c_str());
#else
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsole)
		{
			SetConsoleTextAttribute(hConsole, (WORD)colorCode);
			std::cout << "[" << levelLabel << "] - " << message;

			SetConsoleTextAttribute(hConsole, 0);
			std::cout << " at " << file << ":" << line << " " << function << std::endl;
		}
#endif

#else
		std::cout << "\033[" << colorCode << "m"
				<< "[" << levelLabel << "] - " << getDateTime() << " - " << message
				<< "\033[0m"
				<< " at " << file << ":" << line << " " << function
				<< std::endl;
#endif
}

void CppLogger::DisablePrintAll()
{
	std::scoped_lock<std::mutex> lock(m_mutex);
	for (auto &t : mTargets) {
		t->disablePrint();
	}
}

void CppLogger::EnablePrintAll()
{
	std::scoped_lock<std::mutex> lock(m_mutex);
	for (auto &t : mTargets) {
		t->enablePrint();
	}
}

std::string CppLogger::getDateTime() {
#ifdef _WIN32
	auto now = std::chrono::system_clock::now();
	std::time_t start_time = std::chrono::system_clock::to_time_t(now);
	tm *buf = new tm();
	localtime_s(buf, &start_time);
	char timedisplay[100];
	size_t len = std::strftime(timedisplay, sizeof(timedisplay), "%Y-%m-%d %H:%M:%S", buf);
	return std::string(timedisplay, len);
#else
	time_t now = time(nullptr);
	struct tm timeStruct;
	char buf[80];
	timeStruct = *localtime(&now);

	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeStruct);

	return buf;
#endif
}
