cmake_minimum_required(VERSION 3.16)
project("PlatformBridge" VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "-- PlatformBridge: Host OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "-- PlatformBridge: Host Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "-- PlatformBridge: Configuring for ${CMAKE_SYSTEM_NAME} platform")

if(TARGET_TYPE STREQUAL "executable")
    message(STATUS "-- PlatformBridge: Building as an executable")
    add_executable("PlatformBridge"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Fonts.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Monitors.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Paths.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp"
    )
    
    target_include_directories("PlatformBridge" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/source"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}"
    )
elseif(TARGET_TYPE STREQUAL "static")
    message(STATUS "-- PlatformBridge: Building as a static library")
    add_library("PlatformBridge" STATIC
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Fonts.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Monitors.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Paths.cpp"
    )
    
    target_include_directories("PlatformBridge" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/source"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}"
    )
    
    install(TARGETS "PlatformBridge"
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/PlatformBridge
    )
elseif(TARGET_TYPE STREQUAL "dynamic")
    message(STATUS "-- PlatformBridge: Building as a dynamic library")
    add_library("PlatformBridge" SHARED
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Fonts.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Monitors.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Paths.cpp"
    )
    
    target_include_directories("PlatformBridge" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/source"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}"
    )
else() #Target_TYPE not defined or unrecognized
    message(STATUS "-- PlatformBridge: Using pre-built static library")
    string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LOWER)

    if(TARGETPLATFORM STREQUAL "x64-Linux")
        set(LIB_NAME "libPlatformBridge.a")
    else() #x64-windows
        set(LIB_NAME "PlatformBridge.lib")
    endif()

    set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/${TARGETPLATFORM}/${BUILD_TYPE_LOWER}/${LIB_NAME}")
    
    # Create an imported library target for the pre-built static library
    add_library("PlatformBridge" STATIC IMPORTED GLOBAL)
    set_target_properties("PlatformBridge" PROPERTIES
        IMPORTED_LOCATION "${LIB_PATH}"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/source;${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}"
    )

    if(NOT EXISTS "${LIB_PATH}")
        message(FATAL_ERROR "-- PlatformBridge: PlatformBridge library not found at: ${LIB_PATH}. Please build the PlatformBridge library or specify TARGET_TYPE to build it as part of this project.")
    endif()
    
    message(STATUS "-- PlatformBridge: Successfully using pre-built PlatformBridge library from: ${LIB_PATH}")
endif()

enable_testing()

#run cmake:  & "C:\Program Files\CMake\bin\cmake.exe" .. CMakeLists.txt -DTARGET_TYPE=static/dynamic/executable
#build: & "C:\Program Files\CMake\bin\cmake.exe" --build . --config Release or Debug
#copy .lib files from build/Debug or Release to the lib folder in PlatformBridge