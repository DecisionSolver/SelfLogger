#include "pch.h"
#include "../src/logger.h"

class LoggerTest : public ::testing::Test
{
};

void output()
{
	Logger_Debug("test debug");
	Logger_Error("test error");	
	Logger_Info("test info");
	Logger_Warn("test warning");
}

void output_f()
{
	Logger_Debug_F("test debug {}", 2);
	Logger_Error_F("test error {}", 2);
	Logger_Info_F("test info {}", 2);
	Logger_Warn_F("test warning {}", 2);
}

TEST(LoggerTest, OUTPUT)
{
	ASSERT_NO_THROW(output()) << "Something wrong with standart output";
}

TEST(LoggerTest, OUTPUT_F)
{
	ASSERT_NO_THROW(output_f()) << "Something wrong with formatted output";
}

TEST(LoggerTest, CRITICAL)
{
	ASSERT_THROW(Logger_Critical("test critical"), std::runtime_error);
	ASSERT_THROW(Logger_Critical_F("test critical {}", 2), std::runtime_error);
}

TEST(LoggerTest, FILE_TARGET)
{
	size_t fileSizeStart = std::filesystem::file_size("test.log");
	output();
	size_t currentSize = std::filesystem::file_size("test.log");
	ASSERT_FALSE(fileSizeStart == currentSize) << "Failed to write in file";
}

TEST(LoggerTest, DisableOrEnablePrints)
{
	size_t fileSizeStart = std::filesystem::file_size("test.log");
	CppLogger::DisablePrintAll();
	output();
	size_t currentSize = std::filesystem::file_size("test.log");
	ASSERT_TRUE(fileSizeStart == currentSize) << "Failed to disable prints";
	CppLogger::EnablePrintAll();
	output();
	currentSize = std::filesystem::file_size("test.log");
	ASSERT_FALSE(fileSizeStart == currentSize) << "Failed to enable prints";
}

int main(int argc, char** argv)
{
	CppLogger::registerTarget(new FileLoggerTarget("test.log", LogLevel::LOG_LEVEL_DEBUG));
	CppLogger::registerTarget(new FileLoggerTarget("test.log", LogLevel::LOG_LEVEL_ERROR));
	CppLogger::registerTarget(new FileLoggerTarget("test.log", LogLevel::LOG_LEVEL_CRITICAL));
	CppLogger::registerTarget(new FileLoggerTarget("test.log", LogLevel::LOG_LEVEL_FATAL));
	CppLogger::registerTarget(new FileLoggerTarget("test.log", LogLevel::LOG_LEVEL_INFO));
	CppLogger::registerTarget(new FileLoggerTarget("test.log", LogLevel::LOG_LEVEL_WARNING));

	CppLogger::registerTarget(new ConsoleLoggerTarget(LogLevel::LOG_LEVEL_DEBUG));
	CppLogger::registerTarget(new ConsoleLoggerTarget(LogLevel::LOG_LEVEL_ERROR));
	CppLogger::registerTarget(new ConsoleLoggerTarget(LogLevel::LOG_LEVEL_CRITICAL));
	CppLogger::registerTarget(new ConsoleLoggerTarget(LogLevel::LOG_LEVEL_FATAL));
	CppLogger::registerTarget(new ConsoleLoggerTarget(LogLevel::LOG_LEVEL_INFO));
	CppLogger::registerTarget(new ConsoleLoggerTarget(LogLevel::LOG_LEVEL_WARNING));

	CppLogger::registerTarget(new MCVCLoggerTarget(LogLevel::LOG_LEVEL_DEBUG));
	CppLogger::registerTarget(new MCVCLoggerTarget(LogLevel::LOG_LEVEL_ERROR));
	CppLogger::registerTarget(new MCVCLoggerTarget(LogLevel::LOG_LEVEL_CRITICAL));
	CppLogger::registerTarget(new MCVCLoggerTarget(LogLevel::LOG_LEVEL_FATAL));
	CppLogger::registerTarget(new MCVCLoggerTarget(LogLevel::LOG_LEVEL_INFO));
	CppLogger::registerTarget(new MCVCLoggerTarget(LogLevel::LOG_LEVEL_WARNING));

	CppLogger::EnablePrintAll();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}