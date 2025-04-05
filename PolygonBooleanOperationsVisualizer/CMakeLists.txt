cmake_minimum_required(VERSION 3.10)
project(CGALBooleanOperations)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(CGAL REQUIRED)
find_package(GMP REQUIRED)
find_package(MPFR REQUIRED)

# Explicitly set Qt5 directory if needed
set(Qt5_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5")
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Qt configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add include directories
include_directories(${CGAL_INCLUDE_DIRS})
include_directories(${GMP_INCLUDE_DIRS})
include_directories(${MPFR_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories("/usr/include/x86_64-linux-gnu/qt5")  # Add this line
include_directories(.)  # Include the root directory

# Define source files
set(SOURCES 
    src/BooleanOperations.cpp
    src/MainWindow.cpp
    main.cpp
)

# Create executable
add_executable(boolean_operations ${SOURCES})

# Link libraries
target_link_libraries(boolean_operations ${CGAL_LIBRARIES} ${GMP_LIBRARIES} ${MPFR_LIBRARIES} Qt5::Widgets)

# Display Qt include directories for debugging
message(STATUS "Qt5 Widgets include dirs: ${Qt5Widgets_INCLUDE_DIRS}")