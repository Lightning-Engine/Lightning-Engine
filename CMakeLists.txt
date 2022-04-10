cmake_minimum_required(VERSION 3.23.0)
include(CheckIncludeFile)

project(Lightning VERSION 0.0.1)

if(MSVC)
    add_compile_options(/Wall)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

check_include_file(dlfcn.h HAS_DLFCN)
check_include_file(windows.h HAS_WIN32)

set(SANITIZE_ADDRESS OFF CACHE BOOL "Enable Address Sanitizer")
set(SANITIZE_MEMORY OFF CACHE BOOL "Enable Memory Sanitizer")
set(SANITIZE_THREAD OFF CACHE BOOL "Enable Thread Sanitizer")
set(SANITIZE_UNDEFINED OFF CACHE BOOL "Enable Undefined Behavior Sanitizer")

set(DL_DLFCN ${HAS_DLFCN} CACHE BOOL "Use dlfcn for dynamic linking")
set(DL_WIN32 ${HAS_WIN32} CACHE BOOL "Use win32 for dynamic linking")

set(SANITIZE_OPTIONS
    $<$<BOOL:${SANITIZE_ADDRESS}>:-fsanitize=address>
    $<$<BOOL:${SANITIZE_MEMORY}>:-fsanitize=memory>
    $<$<BOOL:${SANITIZE_THREAD}>:-fsanitize=thread>
    $<$<BOOL:${SANITIZE_UNDEFINED}>:-fsanitize=undefined>
)

add_compile_options(${SANITIZE_OPTIONS})
add_link_options(${SANITIZE_OPTIONS})

file(GLOB_RECURSE FILES engine/* sandbox/*)
add_library(Lightning engine/src/dl.c)
add_executable(Sandbox sandbox/src/main.c)

target_compile_definitions(Lightning PRIVATE
    $<$<BOOL:${DL_DLFCN}>:LI_DL_DLFCN=1>
    $<$<BOOL:${DL_WIN32}>:LI_DL_WIN32=1>
)

target_include_directories(Lightning PUBLIC
    ${CMAKE_SOURCE_DIR}/engine/include
)

target_include_directories(Sandbox PRIVATE
    ${CMAKE_SOURCE_DIR}/sandbox/include
)

target_link_libraries(Sandbox PRIVATE Lightning)

add_custom_target(fmt ALL
    COMMAND clang-format -i ${FILES}
    COMMENT "Formatting code"
)

add_custom_target(doc ALL
    COMMAND doxygen
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating documentation"
)

add_custom_target(check ALL
    COMMAND CodeChecker analyze --enable-all --ctu ${CMAKE_BINARY_DIR}/compile_commands.json -o ${CMAKE_BINARY_DIR}/reports
    COMMAND CodeChecker parse ${CMAKE_BINARY_DIR}/reports -e html -o ${CMAKE_BINARY_DIR}/reports
    COMMENT "Checking code"
)