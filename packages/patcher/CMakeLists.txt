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

# Include directories
target_include_directories(${PATCHER_EXECUTABLE} PRIVATE
    src
    include
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
