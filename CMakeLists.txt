cmake_minimum_required(VERSION 3.16)
project("PlatformBridge" VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "-- PlatformBridge: Host OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "-- PlatformBridge: Host Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "-- PlatformBridge: Configuring for ${CMAKE_SYSTEM_NAME} platform")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "-- PlatformBridge: Configuring for Debug build")
endif()

if(NOT DEFINED TARGET_TYPE OR TARGET_TYPE STREQUAL "")
    set(TARGET_TYPE "static")
    message(STATUS "-- PlatformBridge: TARGET_TYPE not specified, defaulting to static")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Fontconfig REQUIRED)
    find_package(X11 REQUIRED)
endif()

if(TARGET_TYPE STREQUAL "executable")
    message(STATUS "-- PlatformBridge: Building as an executable")
    add_executable("PlatformBridge"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Clipboard.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Fonts.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Input.cpp"
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
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Clipboard.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Fonts.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Input.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Monitors.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Paths.cpp"
    )
    
    target_include_directories("PlatformBridge" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/source"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}"
    )

    add_custom_command(TARGET "PlatformBridge" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/x64-${CMAKE_SYSTEM_NAME}/$<CONFIG>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:PlatformBridge>"
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/x64-${CMAKE_SYSTEM_NAME}/$<CONFIG>"
    )
    
    install(TARGETS "PlatformBridge"
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/PlatformBridge
    )
elseif(TARGET_TYPE STREQUAL "dynamic")
    message(STATUS "-- PlatformBridge: Building as a dynamic library")
    add_library("PlatformBridge" SHARED
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Clipboard.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Fonts.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Input.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Monitors.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}/Paths.cpp"
    )
    
    target_include_directories("PlatformBridge" PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/source"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/Impl/${CMAKE_SYSTEM_NAME}"
    )
else() #Target_TYPE not defined or unrecognized
    message(STATUS "-- PlatformBridge: Using pre-built static library")

    if(TARGETPLATFORM STREQUAL "x64-Linux")
        set(LIB_NAME "libPlatformBridge.a")
    else() #x64-windows
        set(LIB_NAME "PlatformBridge.lib")
    endif()

    set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/${TARGETPLATFORM}/${CMAKE_BUILD_TYPE}/${LIB_NAME}")
    
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

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(TARGET_TYPE STREQUAL "executable")
        target_link_libraries("PlatformBridge" PRIVATE
            Fontconfig::Fontconfig
            ${X11_LIBRARIES} Xtst Xi)


            target_include_directories("PlatformBridge" PRIVATE ${X11_INCLUDE_DIR})
    else()
        target_link_libraries("PlatformBridge" INTERFACE
            Fontconfig::Fontconfig
            ${X11_LIBRARIES} Xtst Xi)

        target_include_directories("PlatformBridge" INTERFACE ${X11_INCLUDE_DIR})
    endif()
endif()

if((TARGET_TYPE STREQUAL "executable") OR (TARGET_TYPE STREQUAL "static") OR (TARGET_TYPE STREQUAL "dynamic"))
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_compile_definitions("PlatformBridge" PUBLIC TARGET_PLATFORM_LINUX)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_compile_definitions("PlatformBridge" PUBLIC TARGET_PLATFORM_WINDOWS)
    endif()
else()
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_compile_definitions("PlatformBridge" INTERFACE TARGET_PLATFORM_LINUX)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_compile_definitions("PlatformBridge" INTERFACE TARGET_PLATFORM_WINDOWS)
    endif()
endif()

enable_testing()

#run cmake:  & "C:\Program Files\CMake\bin\cmake.exe" .. CMakeLists.txt -DTARGET_TYPE=static/dynamic/executable -DCMAKE_BUILD_TYPE=Release/Debug
#build: & "C:\Program Files\CMake\bin\cmake.exe" --build . --config Release or Debug
#copy .lib files from build/Debug or Release to the lib folder in PlatformBridge