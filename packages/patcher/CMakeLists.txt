# Velcro Patcher
project(velcro-patcher)

# Set the executable name
set(PATCHER_EXECUTABLE velcrop)

# Find source files
file(GLOB_RECURSE PATCHER_SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

# Create the executable
add_executable(${PATCHER_EXECUTABLE} ${PATCHER_SOURCES})

# Set compile flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    separate_arguments(DEBUG_FLAGS UNIX_COMMAND "${CMAKE_CXX_FLAGS_DEBUG}")
    target_compile_options(${PATCHER_EXECUTABLE} PRIVATE ${DEBUG_FLAGS})
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    separate_arguments(RELEASE_FLAGS UNIX_COMMAND "${CMAKE_CXX_FLAGS_RELEASE}")
    target_compile_options(${PATCHER_EXECUTABLE} PRIVATE ${RELEASE_FLAGS})
endif()

# Include directories
target_include_directories(${PATCHER_EXECUTABLE} PRIVATE
    src
    include
    ../common/include
)

# Set target properties
set_target_properties(${PATCHER_EXECUTABLE} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Enable testing
enable_testing()

# Add test subdirectory if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
    add_subdirectory(test)
endif()
