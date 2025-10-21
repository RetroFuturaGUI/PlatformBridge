cmake_minimum_required(VERSION 3.16)
project("PlatformBridge" VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Host OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Host Processor: ${CMAKE_SYSTEM_PROCESSOR}")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Configuring for Windows platform")
    
    add_library("PlatformBridge" STATIC
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Windows/Monitors.cpp"
    )

    target_include_directories("PlatformBridge" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/source"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Windows"
    )


else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

install(TARGETS "PlatformBridge"
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/PlatformBridge
)

enable_testing()

#run cmake:  & "C:\Program Files\CMake\bin\cmake.exe" .. CMakeLists.txt -DCMAKE_BUILD_TYPE=Release
#build: & "C:\Program Files\CMake\bin\cmake.exe" --build . --config Release or Debug
#copy .lib files from build/Debug or Release to the lib folder in PlatformBridge