cmake_minimum_required(VERSION 3.0)
project(Hect CXX)

set(HECT_MAJOR_VERSION 0)
set(HECT_MINOR_VERSION 0)
set(HECT_PATCH_VERSION 1)
set(HECT_VERSION ${HECT_MAJOR_VERSION}.${HECT_MINOR_VERSION}.${HECT_PATCH_VERSION})

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/Output" CACHE PATH "Output directory for built files")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
endforeach()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/Bullet")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/ENet")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/Glew")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/JsonCpp")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/LodePng")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/PhysFS")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/SDL2")

add_subdirectory("${PROJECT_SOURCE_DIR}/Engine")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Tests/UnitTests")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Tests/SystemTests")

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