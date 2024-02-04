cmake_minimum_required(VERSION 3.22.1)

project(
    exi-vm
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

if(MSVC)
# warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
# lots of warnings and all warnings as errors
    add_compile_options(
        -Wall
        -Wextra
        -pedantic
        -Werror
        -fsanitize=address
        -Wno-nested-anon-types
        -Wno-gnu-anonymous-struct
        -D_FORTIFY_SOURCE=2
        -D_GLIBCXX_ASSERTIONS
        -fasynchronous-unwind-tables
        -fstack-protector-all
        -fstack-protector-strong
        -O2
        -pipe
        -fpie
    )
    add_link_options(
        -fsanitize=address
    )
endif()


include_directories(include)

include(FetchContent)

FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(GSL)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/lc3.cpp
    src/cpu.cpp
    src/instructions.cpp
)

find_package(fmt)

target_link_libraries(
  ${PROJECT_NAME}
  fmt::fmt
  Microsoft.GSL::GSL
)
