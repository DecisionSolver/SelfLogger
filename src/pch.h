#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <stdio.h>
#define FMT_HEADER_ONLY
#include "fmt/include/fmt/format.h"
#include "fmt/include/fmt/core.h"

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

#endif //PCH_H
