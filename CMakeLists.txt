cmake_minimum_required(VERSION 3.17)
project(z5)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

include_directories(inc)
include_directories(src)

add_executable(z5
        inc/OpenGL_API.hh
        inc/Draw3D_api_interface.hh
        inc/dron.hh
        inc/tereny.hh
        inc/ostroslup.hh
        inc/Prostokat.hh
        inc/Wektor.hh
        inc/Brylka.hh
        src/ostroslup.cpp
        src/Brylka.cpp
        src/OpenGL_API.cpp
        src/tereny.cpp
        src/dron.cpp
        src/main.cpp
        src/Prostokat.cpp
        src/Wektor.cpp)

target_link_libraries(z5
        ${OPENGL_LIBRARY} # filled by "find_package(OpenGL REQUIRED)"
        ${GLUT_LIBRARY}
        )