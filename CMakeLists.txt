cmake_minimum_required(VERSION 2.8)
project(Hect CXX)

enable_language(C)

set(HECT_MAJOR_VERSION 0)
set(HECT_MINOR_VERSION 0)
set(HECT_PATCH_VERSION 1)
set(HECT_VERSION ${HECT_MAJOR_VERSION}.${HECT_MINOR_VERSION}.${HECT_PATCH_VERSION})

option(HEADLESS "Whether Hect should be built without SDL and OpenGL (useful for testing or server builds)" OFF)
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/Output" CACHE PATH "Output directory for built files")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
endforeach()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Macros
macro(grouped_source_files SOURCE_FILES SOURCE_DIR)
    file(GLOB ROOT_FILES ${SOURCE_DIR}/*)
    source_group(Source FILES ${ROOT_FILES})
    file(GLOB_RECURSE CHILDREN RELATIVE ${SOURCE_DIR} ${SOURCE_DIR}/*)
    SET(DIR_LIST "")
    foreach(CHILD ${CHILDREN})
        get_filename_component(DIR_PATH ${CHILD} PATH)
        list(APPEND DIR_LIST ${DIR_PATH})
    endforeach()
    list(REMOVE_DUPLICATES DIR_LIST)
    foreach(REL_DIR ${DIR_LIST})
        file(GLOB GROUPED_FILES ${SOURCE_DIR}/${REL_DIR}/*)
        string(REPLACE "/" "\\\\" GROUP "${REL_DIR}")
        source_group("Source\\\\${GROUP}" FILES ${GROUPED_FILES})
    endforeach()
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.h ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.inl)
endmacro()
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
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/LodePng")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/PhysFS")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Dependencies/YamlCpp")

# Add Hect tests
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Tests/UnitTests")
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine/Tests/SystemTests")

# Add Hect projects
subdirectory_list(PROJECT_DIRS ${PROJECT_SOURCE_DIR}/Projects)
foreach(PROJECT_DIR ${PROJECT_DIRS})
    add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/${PROJECT_DIR})
endforeach()
