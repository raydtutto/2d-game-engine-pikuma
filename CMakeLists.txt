cmake_minimum_required(VERSION 3.23)
project(gameengine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Lua REQUIRED)

include_directories(libs)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS} ${LUA_INCLUDE_DIR})
#message(FATAL_ERROR ${LUA_INCLUDE_DIR})

file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
list(APPEND
        ${CMAKE_SOURCE_DIR}/libs/sol/sol.hpp
        ${CMAKE_SOURCE_DIR}/libs/sol/config.hpp
        ${CMAKE_SOURCE_DIR}/libs/sol/forward.hpp
)

add_executable(gameengine ${HEADERS} ${SOURCES})
target_link_libraries(gameengine ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${LUA_LIBRARIES})