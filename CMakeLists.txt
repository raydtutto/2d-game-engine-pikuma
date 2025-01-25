cmake_minimum_required(VERSION 3.23)

#-------------------------------------------------------------------------------------
# Set VCPKG
#-------------------------------------------------------------------------------------

set(VCPKG_TOOLCHAIN_ROOT $ENV{VCPKG_ROOT})
if (${VCPKG_TOOLCHAIN_ROOT} STREQUAL "")
    set(VCPKG_TOOLCHAIN_ROOT "~/vcpkg")
    message(WARNING "Environment didn't set up. Using default VCPKG root.")
endif ()
set(VCPKG_TOOLCHAIN_ROOT "${VCPKG_TOOLCHAIN_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_TOOLCHAIN_ROOT})

#-------------------------------------------------------------------------------------
# Set project
#-------------------------------------------------------------------------------------

project(gameengine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
#${CMAKE_SOURCE_DIR}
#${CMAKE_CURRENT_SOURCE_DIR}
#${CMAKE_BINARY_DIR}

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Lua REQUIRED)
find_package(spdlog CONFIG REQUIRED)

include_directories(libs)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS} ${LUA_INCLUDE_DIR})

# Searches recursively through all subdirectories
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
set(LIBS_SOURCES)
add_subdirectory(libs)

add_executable(gameengine ${HEADERS} ${SOURCES} ${LIBS_SOURCES})
target_link_libraries(gameengine
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${LUA_LIBRARIES}
        spdlog::spdlog_header_only
)