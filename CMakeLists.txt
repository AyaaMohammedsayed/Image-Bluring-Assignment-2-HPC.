cmake_minimum_required(VERSION 3.10)
project(MPICVBlur)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)

# Find MPI (this handles all flags)
find_package(MPI REQUIRED)
include_directories(${MPI_CXX_INCLUDE_PATH})

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# The executable and its sources
add_executable(mpi_cv_blur 
    src/main.cpp 
    src/cli.cpp
    src/data_manager.cpp 
    src/mpi_manager.cpp 
    src/blur_processor.cpp
)

# Link MPI and OpenCV to the executable
target_link_libraries(mpi_cv_blur ${MPI_CXX_LIBRARIES} ${OpenCV_LIBRARIES})