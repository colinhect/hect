cmake_minimum_required(VERSION 3.0)
project(Hect CXX)

enable_language(C)

set(HECT_MAJOR_VERSION 0)
set(HECT_MINOR_VERSION 0)
set(HECT_PATCH_VERSION 1)
set(HECT_VERSION ${HECT_MAJOR_VERSION}.${HECT_MINOR_VERSION}.${HECT_PATCH_VERSION})

set(HECT_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Output CACHE PATH "Output directory for built files")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HECT_OUTPUT_DIR})

foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${HECT_OUTPUT_DIR})
endforeach()

option(HECT_HEADLESS "Whether Hect should be built without SDL and OpenGL (useful for testing or server builds)" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Engine/Dependencies/CMake)

add_subdirectory(${PROJECT_SOURCE_DIR}/Engine)

# Add Hect dependencies
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/Bullet)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/Catch)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/Celero)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/ENet)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/Glew)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/LibYaml)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/LodePng)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/Lua)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/NanoVG)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/PhysFS)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/Selene)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Dependencies/Tclap)

# Add Hect tools
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Tools/Build)

# Add Hect tests
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Tests/PerformanceTests)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Tests/SystemTests)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Tests/UnitTests)

# Add Hect examples
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Examples/HelloWorld)

# Add Hect projects
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
subdirectory_list(PROJECT_DIRS ${PROJECT_SOURCE_DIR}/Projects)
foreach(PROJECT_DIR ${PROJECT_DIRS})
#    add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/${PROJECT_DIR})
endforeach()
