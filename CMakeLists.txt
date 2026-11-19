cmake_minimum_required(VERSION 3.20)

project(Sketchpad3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)   
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(raylib)

file(GLOB_RECURSE SKETCHPAD_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(Sketchpad3D ${SKETCHPAD_SOURCES})

target_include_directories(Sketchpad3D PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(Sketchpad3D PRIVATE raylib)

if(WIN32)
    target_link_libraries(Sketchpad3D PRIVATE dwmapi)
endif()

# предупреждения компилятора
if(MSVC)
    target_compile_options(Sketchpad3D PRIVATE /W4 /utf-8)
else()
    target_compile_options(Sketchpad3D PRIVATE -Wall -Wextra)
endif()

if(MINGW)
    target_link_options(Sketchpad3D PRIVATE -static -static-libgcc -static-libstdc++)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    add_custom_command(TARGET Sketchpad3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/assets
                $<TARGET_FILE_DIR:Sketchpad3D>/assets)
endif()
