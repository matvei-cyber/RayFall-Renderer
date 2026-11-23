cmake_minimum_required (VERSION 4.0)

project ("RayFall-Renderer" LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/src/*.cpp")

add_executable (${PROJECT_NAME} ${SOURCES})

target_include_directories (${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/source/include"
)

target_link_libraries (${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/source/src/libsodium.lib"
    "${CMAKE_SOURCE_DIR}/source/src/raylib.lib"
    winmm
    gdi32
    opengl32
    shell32
    user32
)

target_compile_definitions (${PROJECT_NAME} PRIVATE SODIUM_STATIC)

target_compile_options (${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:
        /O2
        /Ob3
        /Oi
        /Ot
        /GL
        /fp:precise
        /arch:AVX2
    >
)

target_link_options (${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:
        /LTCG
        /OPT:REF
        /OPT:ICF
    >
)
