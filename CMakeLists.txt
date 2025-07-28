cmake_minimum_required(VERSION 3.10)
project(Dattatypes VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library name
set(LIBRARY_NAME "Dattatypes")

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Add Source Files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Make Library
add_library(${LIBRARY_NAME} ${SOURCES})

# Include Directories
target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Enable warnings
target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra -Wpedantic)

# # Export Targets for build tree
# export(EXPORT ${LIBRARY_NAME}Targets
#     FILE "${CMAKE_BINARY_DIR}/${LIBRARY_NAME}Targets.cmake"
#     NAMESPACE ${LIBRARY_NAME}::
# )

# Tests
enable_testing()
add_subdirectory(tests)

# Installation Rules
install(TARGETS ${LIBRARY_NAME}
    EXPORT ${LIBRARY_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

# Export Targets for install tree
install(EXPORT ${LIBRARY_NAME}Targets
    FILE ${LIBRARY_NAME}Targets.cmake
    NAMESPACE ${LIBRARY_NAME}::
    DESTINATION lib/cmake/${LIBRARY_NAME}
)

# Generate Config File for find_package()
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${LIBRARY_NAME}
)

# Install Config File
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake"
    DESTINATION lib/cmake/${LIBRARY_NAME}
)
