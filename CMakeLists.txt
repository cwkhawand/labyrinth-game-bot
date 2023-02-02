cmake_minimum_required(VERSION 3.16)
project(Project C)

set(CMAKE_C_STANDARD 11)

add_library(clientAPI lib/Labyrinth/clientAPI.c lib/Labyrinth/clientAPI.h)
add_library(labyrinthAPI lib/Labyrinth/labyrinthAPI.c lib/Labyrinth/labyrinthAPI.h)
add_library(Labyrinth src/Labyrinth.c src/Labyrinth.h)
add_executable(Project main.c)
target_link_libraries(labyrinthAPI clientAPI)
target_link_libraries(Labyrinth labyrinthAPI)
target_link_libraries(Project labyrinthAPI Labyrinth m)