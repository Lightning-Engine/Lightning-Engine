cmake_minimum_required(VERSION 3.16.0)
include(CheckIncludeFile)

project(Lightning VERSION 0.0.0 LANGUAGES C)

if(MSVC)
    add_compile_options(/W4)
elseif(APPLE)
    add_compile_options(-Wno-deprecated-declarations)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_OBJC_STANDARD 99)
else()
    set(CMAKE_C_STANDARD 90)
    set(CMAKE_OBJC_STANDARD 90)
endif()

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_OBJC_STANDARD_REQUIRED ON)
set(CMAKE_OBJC_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

check_include_file(dlfcn.h HAS_DLFCN)
check_include_file(windows.h HAS_WIN32)
check_include_file(mach-o/dyld.h HAS_DYLD)
check_include_file(X11/Xlib.h HAS_XLIB)
check_include_file(objc/objc-runtime.h HAS_OBJC)
check_include_file(${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.h HAS_ANDROID)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(HAS_IOS ON)
else()
    set(HAS_IOS OFF)
endif()

if(HAS_OBJC)
    enable_language(OBJC)
endif()

# TODO: use these libraries for HAS_* checks
find_library(LIB_COCOA Cocoa)
find_library(LIB_UIKIT UIKit)

set(SANITIZE_ADDRESS OFF CACHE BOOL "Enable Address Sanitizer")
set(SANITIZE_MEMORY OFF CACHE BOOL "Enable Memory Sanitizer")
set(SANITIZE_THREAD OFF CACHE BOOL "Enable Thread Sanitizer")
set(SANITIZE_UNDEFINED OFF CACHE BOOL "Enable Undefined Behavior Sanitizer")

set(DL_DLFCN ${HAS_DLFCN} CACHE BOOL "Use dlfcn for dynamic linking")
set(DL_WIN32 ${HAS_WIN32} CACHE BOOL "Use win32 for dynamic linking")
set(DL_DYLD ${HAS_DYLD} CACHE BOOL "Use dyld for dynamic linking")
set(WIN_XLIB ${HAS_XLIB} CACHE BOOL "Use Xlib for windowing")
set(WIN_WIN32 ${HAS_WIN32} CACHE BOOL "Use win32 for windowing")
set(WIN_COCOA ${HAS_OBJC} CACHE BOOL "Use Cocoa for windowing")
set(MAIN_MAIN $<BOOL:$<NOT:$<OR:$<BOOL:${HAS_ANDROID}>,$<BOOL:${HAS_IOS}>>>> CACHE BOOL "Use main for main")
set(MAIN_ANDROID ${HAS_ANDROID} CACHE BOOL "Use android for main")
set(MAIN_IOS ${HAS_IOS} CACHE BOOL "Use IOS for main")
set(VIEW_ANDROID ${HAS_ANDROID} CACHE BOOL "Use android for views")
set(VIEW_IOS ${HAS_IOS} CACHE BOOL "Use IOS for views")

set(MAIN_SHARED ${MAIN_ANDROID})

set(SANITIZE_OPTIONS
    $<$<BOOL:${SANITIZE_ADDRESS}>:-fsanitize=address>
    $<$<BOOL:${SANITIZE_MEMORY}>:-fsanitize=memory>
    $<$<BOOL:${SANITIZE_THREAD}>:-fsanitize=thread>
    $<$<BOOL:${SANITIZE_UNDEFINED}>:-fsanitize=undefined>
)

add_compile_options(${SANITIZE_OPTIONS})
add_link_options(${SANITIZE_OPTIONS})

file(GLOB_RECURSE FILES engine/* sandbox/*)
add_library(Lightning
    engine/src/std.c
    engine/src/dl/impl.c
    $<$<BOOL:${DL_DLFCN}>:engine/src/dl/dlfcn.c>
    $<$<BOOL:${DL_WIN32}>:engine/src/dl/win32.c>
    $<$<BOOL:${DL_DYLD}>:engine/src/dl/dyld.c>
    engine/src/win/impl.c
    $<$<BOOL:${WIN_XLIB}>:engine/src/win/xlib.c>
    $<$<BOOL:${WIN_WIN32}>:engine/src/win/win32.c>
    $<$<BOOL:${WIN_COCOA}>:engine/src/win/cocoa.m>
    $<$<BOOL:${MAIN_ANDROID}>:engine/src/main/android.c>
    engine/src/view/impl.c
    $<$<BOOL:${VIEW_ANDROID}>:engine/src/view/android.c>
    $<$<BOOL:${VIEW_IOS}>:engine/src/view/ios.m>
)

add_library(Sandbox sandbox/src/main.c)

set(MAIN_FILES
    $<$<BOOL:${MAIN_MAIN}>:engine/src/main/main_main.c>
    $<$<BOOL:${MAIN_ANDROID}>:engine/src/main/main_android.c>
    $<$<BOOL:${MAIN_ANDROID}>:${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c>
    $<$<BOOL:${MAIN_IOS}>:engine/src/main/main_ios.m>
)

if(MAIN_SHARED)
    add_library(Main SHARED ${MAIN_FILES})
else()
    add_executable(Main ${MAIN_FILES})
endif()

target_compile_definitions(Lightning PRIVATE
    $<$<BOOL:${DL_DLFCN}>:LI_DL_DLFCN=1>
    $<$<BOOL:${DL_WIN32}>:LI_DL_WIN32=1>
    $<$<BOOL:${DL_DYLD}>:LI_DL_DYLD=1>
    $<$<BOOL:${WIN_XLIB}>:LI_WIN_XLIB=1>
    $<$<BOOL:${WIN_WIN32}>:LI_WIN_WIN32=1>
    $<$<BOOL:${WIN_COCOA}>:LI_WIN_COCOA=1>
    $<$<BOOL:${VIEW_ANDROID}>:LI_VIEW_ANDROID=1>
    $<$<BOOL:${VIEW_IOS}>:LI_VIEW_IOS=1>
)

target_include_directories(Lightning PUBLIC
    ${CMAKE_SOURCE_DIR}/engine/include
    $<$<BOOL:${MAIN_ANDROID}>:${ANDROID_NDK}/sources/android/native_app_glue>
)

target_include_directories(Sandbox PRIVATE
    ${CMAKE_SOURCE_DIR}/sandbox/include
)

target_include_directories(Main PRIVATE
    $<$<BOOL:${MAIN_ANDROID}>:${ANDROID_NDK}/sources/android/native_app_glue>
)

target_link_libraries(Lightning PRIVATE
    $<$<BOOL:${WIN_XLIB}>:X11>
    $<$<BOOL:${WIN_COCOA}>:${LIB_COCOA}>
    $<$<BOOL:${VIEW_IOS}>:${LIB_UIKIT}>
)

target_link_libraries(Sandbox PRIVATE Lightning)
target_link_libraries(Main PRIVATE Lightning Sandbox)

if(HAS_ANDROID)
    find_library(LIB_ANDROID android)
    find_library(LIB_LOG log)
    target_link_libraries(Main PRIVATE ${LIB_ANDROID} ${LIB_LOG})
endif()

add_custom_target(fmt
    COMMAND clang-format -i ${FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Formatting code"
)

add_custom_target(doc
    COMMAND doxygen
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating documentation"
)

add_custom_target(check
    COMMAND CodeChecker analyze --enable-all --ctu ${CMAKE_BINARY_DIR}/compile_commands.json -o ${CMAKE_BINARY_DIR}/reports
    COMMAND CodeChecker parse ${CMAKE_BINARY_DIR}/reports -e html -o ${CMAKE_BINARY_DIR}/reports
    COMMENT "Checking code"
)
