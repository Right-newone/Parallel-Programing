cmake_minimum_required(VERSION 4.3)

project(Lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Lab1 main.cpp)

target_sources(Lab1
    PRIVATE
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
            matrix_core_n_files.cppm
            matrix_ops.cppm
)

if(MSVC)

    target_compile_options(Lab1
        PRIVATE
            /W4
            /EHsc
    )

else()

    target_compile_options(Lab1
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix_a.txt
    ${CMAKE_CURRENT_BINARY_DIR}/matrix_a.txt
    COPYONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix_b.txt
    ${CMAKE_CURRENT_BINARY_DIR}/matrix_b.txt
    COPYONLY
)