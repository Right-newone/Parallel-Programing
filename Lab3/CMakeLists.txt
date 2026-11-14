cmake_minimum_required(VERSION 3.16)
project("Parallel Programming" CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories("D:/Данные/MPI/Include/Include")
link_directories("D:/Данные/MPI/Include/Lib/x64")

add_executable(matrix_mpi main.cpp)
target_link_libraries(matrix_mpi PRIVATE msmpi)