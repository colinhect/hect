cmake_minimum_required(VERSION 3.0)
project(Hect CXX)

set(HECT_MAJOR_VERSION 0)
set(HECT_MINOR_VERSION 0)
set(HECT_PATCH_VERSION 1)
set(HECT_VERSION ${HECT_MAJOR_VERSION}.${HECT_MINOR_VERSION}.${HECT_PATCH_VERSION})

set(HECT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Output" CACHE PATH "Output directory for built files")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HECT_OUTPUT_DIR})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${HECT_OUTPUT_DIR})
endforeach()

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

macro(SUBDIRLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

SUBDIRLIST(PROJECT_DIRS ${PROJECT_SOURCE_DIR}/Projects)

foreach(PROJECT_DIR ${PROJECT_DIRS})
    add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/${PROJECT_DIR})
endforeach()