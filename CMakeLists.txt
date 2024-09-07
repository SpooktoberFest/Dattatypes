cmake_minimum_required(VERSION 3.10)
project(Dattatypes)
set(EXECUTABLE_NAME "${PROJECT_NAME}.exe")

set(CMAKE_CXX_STANDARD 11)

# Find Libraries
find_package(Eigen3 REQUIRED)

# Add Source Files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Make Executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Include Libraries
target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/h
    ${EIGEN3_INCLUDE_DIR}
)
