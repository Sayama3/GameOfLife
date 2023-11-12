cmake_minimum_required(VERSION 3.23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libraries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libraries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

set(GAME_NAME GameOfLife)
set(FRAMEWORK_NAME Voxengine)

project(${GAME_NAME})

add_subdirectory(lib/Voxengine)

target_include_directories(${FRAMEWORK_NAME} PUBLIC include)
target_sources(${FRAMEWORK_NAME} PUBLIC
        include/GameOfLifeSystem.hpp
        src/GameOfLifeSystem.cpp
        include/GameOfLifeComponent.hpp
        src/GameOfLifeComponent.cpp
        include/CameraController.hpp
        src/CameraController.cpp
)

target_compile_definitions(${FRAMEWORK_NAME} PUBLIC GAME_NAME="${GAME_NAME}")

file(COPY Assets/ DESTINATION ${CMAKE_BINARY_DIR}/Assets/)
file(COPY Systems/ DESTINATION ${CMAKE_BINARY_DIR}/Systems/)