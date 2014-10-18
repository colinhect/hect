cmake_minimum_required(VERSION 3.0)
project(Hect CXX)

set(HECT_MAJOR_VERSION 0)
set(HECT_MINOR_VERSION 0)
set(HECT_PATCH_VERSION 1)
set(HECT_VERSION ${HECT_MAJOR_VERSION}.${HECT_MINOR_VERSION}.${HECT_PATCH_VERSION})

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

set(HECT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HECT_OUTPUT_DIR})

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/Hect)
endif()

foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/Bullet")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/ENet")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/Glew")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/JsonCpp")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/LodePng")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/PhysFS")
add_subdirectory("${PROJECT_SOURCE_DIR}/Dependencies/SDL2")

add_subdirectory("${PROJECT_SOURCE_DIR}/Engine")
add_subdirectory("${PROJECT_SOURCE_DIR}/Tests/UnitTests")
add_subdirectory("${PROJECT_SOURCE_DIR}/Tests/SystemTests")