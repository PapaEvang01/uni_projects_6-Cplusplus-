cmake_minimum_required(VERSION 4.0)
project(SmartSelfie)

# Find installed OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Add your main source file
add_executable(SmartSelfie main.cpp)

# Link OpenCV libraries to your executable
target_link_libraries(SmartSelfie ${OpenCV_LIBS})
