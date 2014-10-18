cmake_minimum_required(VERSION 3.0)
project(Hect CXX)

set(HECT_MAJOR_VERSION 0)
set(HECT_MINOR_VERSION 0)
set(HECT_PATCH_VERSION 1)
set(HECT_VERSION ${HECT_MAJOR_VERSION}.${HECT_MINOR_VERSION}.${HECT_PATCH_VERSION})

set(HECT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Output" CACHE PATH "Output directory for built files")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HECT_OUTPUT_DIR})

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/Bullet")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/ENet")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/Glew")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/JsonCpp")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/LodePng")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/PhysFS")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/SDL2")

add_subdirectory("${PROJECT_SOURCE_DIR}/Hect")
add_subdirectory("${PROJECT_SOURCE_DIR}/Hect/Tests/UnitTests")
add_subdirectory("${PROJECT_SOURCE_DIR}/Hect/Tests/SystemTests")