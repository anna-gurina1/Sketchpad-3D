cmake_minimum_required(VERSION 3.20)

project(Sketchpad3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# raylib: скачивается автоматически при первой конфигурации (нужен интернет)
# ---------------------------------------------------------------------------
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)   # не собирать примеры raylib
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(raylib)

# ---------------------------------------------------------------------------
# Программа: все .cpp из папки src/ подключаются автоматически,
# поэтому новый файл достаточно просто создать в src/
# ---------------------------------------------------------------------------
file(GLOB_RECURSE SKETCHPAD_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(Sketchpad3D ${SKETCHPAD_SOURCES})

target_include_directories(Sketchpad3D PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(Sketchpad3D PRIVATE raylib)

# предупреждения компилятора
if(MSVC)
    target_compile_options(Sketchpad3D PRIVATE /W4 /utf-8)
else()
    target_compile_options(Sketchpad3D PRIVATE -Wall -Wextra)
endif()

# MinGW: стандартные библиотеки вшиваются в exe,
# чтобы программа запускалась двойным щелчком, а не только из терминала MSYS
if(MINGW)
    target_link_options(Sketchpad3D PRIVATE -static -static-libgcc -static-libstdc++)
endif()

# ---------------------------------------------------------------------------
# После каждой сборки папка assets/ (модели OBJ) копируется рядом с exe
# ---------------------------------------------------------------------------
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    add_custom_command(TARGET Sketchpad3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/assets
                $<TARGET_FILE_DIR:Sketchpad3D>/assets)
endif()
