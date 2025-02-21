cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find OpenCV package (must be installed on the system)
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Create the executable
add_executable(asciiGen src/main.cpp)

# Link OpenCV libraries
target_compile_features(asciiGen PRIVATE cxx_std_17)
target_link_libraries(asciiGen PRIVATE ${OpenCV_LIBS})

# Print OpenCV include and library paths for debugging
message(STATUS "OpenCV Include Directories: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV Libraries: ${OpenCV_LIBS}")
