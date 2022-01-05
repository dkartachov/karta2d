cmake_minimum_required(VERSION 3.22.1)

project(Karta2D)

include_directories(include include/Components)

set(SOURCE_LIST
    "src/main.cpp"
    "src/Application.cpp"
    "src/Graphics.cpp"
    "src/Timer.cpp"
)

set(HEADER_LIST
    "include/Application.h"
    "include/Graphics.h"
    "include/Timer.h"
    "include/SimpleECS.h"
    "include/EntityManager.h"
    "include/Collision2D.h"
    "include/Vector2D.h"
    # Components
    "include/Components/Components.h"
    "include/Components/Box.h"
    "include/Components/BoxCollider2D.h"
    "include/Components/Circle.h"
    "include/Components/CircleCollider2D.h"
    "include/Components/Rigidbody2D.h"
    "include/Components/Transform2D.h"
)

add_executable(${PROJECT_NAME} ${SOURCE_LIST} ${HEADER_LIST})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST} ${HEADER_LIST})

# Find and link SDL2 libraries
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Main)