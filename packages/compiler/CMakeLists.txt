# Velcro Compiler
project(velcro-compiler)

# Set the executable name
set(COMPILER_EXECUTABLE velcroc)

# Find source files
file(GLOB_RECURSE COMPILER_SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

# Create the executable
add_executable(${COMPILER_EXECUTABLE} ${COMPILER_SOURCES})

# Set compile flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    separate_arguments(DEBUG_FLAGS UNIX_COMMAND "${CMAKE_CXX_FLAGS_DEBUG}")
    target_compile_options(${COMPILER_EXECUTABLE} PRIVATE ${DEBUG_FLAGS})
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    separate_arguments(RELEASE_FLAGS UNIX_COMMAND "${CMAKE_CXX_FLAGS_RELEASE}")
    target_compile_options(${COMPILER_EXECUTABLE} PRIVATE ${RELEASE_FLAGS})
endif()

# Include directories
target_include_directories(${COMPILER_EXECUTABLE} PRIVATE
    src
    include
)

# Set target properties
set_target_properties(${COMPILER_EXECUTABLE} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Enable testing
enable_testing()

# Add test subdirectory if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
    add_subdirectory(test)
endif()
