cmake_minimum_required(VERSION 3.19)
project(Voronoi)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(CMAKE_CXX_STANDARD 11)

add_executable(Voronoi
        Voronoi.cpp
        main.cpp)

target_link_libraries(Voronoi "-framework OpenGL -framework GLUT")
