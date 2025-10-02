# Velcro Decompiler
project(velcro-decompiler)

# Set the executable name
set(DECOMPILER_EXECUTABLE velcrod)

# Find source files
file(GLOB_RECURSE DECOMPILER_SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

# Create the executable
add_executable(${DECOMPILER_EXECUTABLE} ${DECOMPILER_SOURCES})

# Include directories
target_include_directories(${DECOMPILER_EXECUTABLE} PRIVATE
    src
    include
)

# Set target properties
set_target_properties(${DECOMPILER_EXECUTABLE} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Enable testing
enable_testing()

# Add test subdirectory if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
    add_subdirectory(test)
endif()
