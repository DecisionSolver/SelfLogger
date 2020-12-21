project(SelfLogger)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} STATIC
        logger.cpp logger.h)