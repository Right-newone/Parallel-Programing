cmake_minimum_required(VERSION 3.28)

project(Lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED COMPONENTS CXX)

add_executable(Lab2 main.cpp)

target_sources(Lab2
    PRIVATE
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
            matrix_core_n_files.cppm
            matrix_ops.cppm
)

target_link_libraries(Lab2 PRIVATE OpenMP::OpenMP_CXX)

if(MSVC)
    target_compile_options(Lab2 PRIVATE /W4 /EHsc)
endif()

file(GLOB MATRIX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/matrix_*.txt)

add_custom_target(copy_matrices ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MATRIX_FILES}
        $<TARGET_FILE_DIR:Lab2>
    COMMENT "Copying matrix files next to Lab2.exe"
    VERBATIM
)

add_dependencies(Lab2 copy_matrices)