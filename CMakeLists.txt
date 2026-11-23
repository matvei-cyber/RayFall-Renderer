cmake_minimum_required (VERSION 4.0)

set (CMAKE_CXX_COMPILER "g++")

project ("RayFall-Renderer" LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 26)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/src/*.cpp")

add_executable (${PROJECT_NAME} ${SOURCES})

target_include_directories (${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/source/include"
)

file(GLOB STATIC_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/source/src/*.a")

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${STATIC_LIBS}
    winmm
    gdi32
    opengl32
)

string (APPEND CMAKE_CXX_FLAGS " -O3 -flto -fno-plt -Wall -Wextra -Wpedantic -Werror -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wimplicit-fallthrough -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast -Wsuggest-override -static")
