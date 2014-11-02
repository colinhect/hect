cmake_minimum_required(VERSION 2.8)
project(Hect CXX)

enable_language(C)

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

# Macros
macro(subdirectory_list result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

add_subdirectory("${PROJECT_SOURCE_DIR}/Engine")

# Add Hect dependencies
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/Bullet")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/ENet")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/Glew")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/JsonCpp")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/LodePng")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/PhysFS")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/SDL2")

# Add Hect tests
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Tests/UnitTests")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Tests/SystemTests")

# Add Hect projects
subdirectory_list(PROJECT_DIRS ${PROJECT_SOURCE_DIR}/Projects)
foreach(PROJECT_DIR ${PROJECT_DIRS})
    add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/${PROJECT_DIR})
endforeach()
