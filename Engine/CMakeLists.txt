if(WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS /DHECT_EXPORTS")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++1y -Wall -Wextra -Werror -pedantic")
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/Engine/Source/Hect/Core/Configuration.h.in"
    "${PROJECT_SOURCE_DIR}/Engine/Source/Hect/Core/Configuration.h"
    )

include_directories(
    "${PROJECT_SOURCE_DIR}/Engine/Source"
    "${PROJECT_SOURCE_DIR}/Engine/Dependencies/Bullet"
    "${PROJECT_SOURCE_DIR}/Engine/Dependencies/ENet"
    "${PROJECT_SOURCE_DIR}/Engine/Dependencies/Glew"
    "${PROJECT_SOURCE_DIR}/Engine/Dependencies/LibYaml"
    "${PROJECT_SOURCE_DIR}/Engine/Dependencies/LodePng"
    "${PROJECT_SOURCE_DIR}/Engine/Dependencies/PhysFS"
    "${PROJECT_SOURCE_DIR}/Engine/Dependencies/Tclap/include"
    )

grouped_source_files(SOURCE_FILES ${PROJECT_SOURCE_DIR}/Engine/Source)
add_library(Hect SHARED ${SOURCE_FILES})

add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Assets)
add_dependencies(Hect HectAssets)

add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Source/Hect/Generated)
add_dependencies(Hect HectTypes)

set(LINK_LIBRARIES Bullet ENet Glew LibYaml LodePng PhysFS)

# Add OpenGL and SDL dependencies if not building headless
if(NOT HEADLESS)

    # OpenGL
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS})
    list(APPEND LINK_LIBRARIES ${OPENGL_LIBRARIES})

    # SDL
    if(UNIX)
        include(FindPkgConfig)
        pkg_search_module(SDL2 required sdl2)
        include_directories(${SDL2_INCLUDE_DIRS})
        list(APPEND LINK_LIBRARIES ${SDL2_LIBRARIES})
    elseif(WIN32)
        set(SDL_DIR "${PROJECT_SOURCE_DIR}/Engine/Dependencies/SDL2-2.0.3")
        include_directories("${SDL_DIR}/include/")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            list(APPEND LINK_LIBRARIES "${SDL_DIR}/lib/x64/SDL2.lib")
            add_custom_command(TARGET Hect
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${SDL_DIR}/lib/x64/SDL2.dll" "${OUTPUT_DIR}/SDL2.dll"
                )
        else()
            list(APPEND LINK_LIBRARIES "${SDL_DIR}/lib/x86/SDL2.lib")
            add_custom_command(TARGET Hect
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${SDL_DIR}/lib/x86/SDL2.dll" "${OUTPUT_DIR}//SDL2.dll"
                )
        endif()
    endif()
endif()

if(UNIX)
    list(APPEND LINK_LIBRARIES "pthread")
elseif(WIN32)
    list(APPEND LINK_LIBRARIES "ws2_32" "winmm")
endif()

target_link_libraries(Hect ${LINK_LIBRARIES})

set_target_properties(Hect PROPERTIES
    PROJECT_LABEL Hect
    LINKER_LANGUAGE CXX
    FOLDER "/Engine"
    )