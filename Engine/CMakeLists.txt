configure_file(
    ${PROJECT_SOURCE_DIR}/Engine/Source/Hect/Core/Configuration.h.in
    ${PROJECT_SOURCE_DIR}/Engine/Source/Hect/Core/Configuration.h
    )

include(Sources.cmake)
add_library(Hect SHARED ${SOURCE_FILES})

target_include_directories(Hect PUBLIC ${PROJECT_SOURCE_DIR}/Engine/Source)

if(MSVC)
    target_compile_options(Hect PRIVATE /W3 /WX /wd /bigobj)
    target_compile_definitions(Hect PRIVATE _CRT_SECURE_NO_WARNINGS HECT_EXPORTS)

    # Visual Leak Detector
    include(CheckIncludeFiles)
    check_include_files(vld.h FOUND_VLD)
    if(FOUND_VLD)
        target_compile_definitions(Hect PUBLIC HECT_USE_VLD)
    endif()
else()
    target_compile_options(Hect PRIVATE -std=c++1y -Wall -Wextra -Werror -pedantic)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Assets)
add_dependencies(Hect HectAssets)

add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/Source/Hect/Generated)
add_dependencies(Hect HectTypes)

target_link_libraries(Hect PRIVATE
    Bullet
    ENet
    Glew
    LibYaml
    LodePng
    NanoVG
    PhysFS
    Lua
    Selene
    Tclap
    )

# Add dependencies that are excluded if building headless
if(NOT HECT_HEADLESS)

    # OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(Hect PRIVATE ${OPENGL_LIBRARIES})

    # SDL
    if(MSVC)
        # Use the included SDL2 SDK
        set(SDL2_PATH ${PROJECT_SOURCE_DIR}/Engine/Dependencies/SDL2-2.0.3)

        # Copy DLLs to binary directory
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            list(APPEND LINK_LIBRARIES "${SDL2_PATH}/lib/x64/SDL2.lib")
            add_custom_command(TARGET Hect
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_PATH}/lib/x64/SDL2.dll" "${HECT_OUTPUT_DIR}/SDL2.dll"
                )
        else()
            list(APPEND LINK_LIBRARIES "${SDL2_PATH}/lib/x86/SDL2.lib")
            add_custom_command(TARGET Hect
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_PATH}/lib/x86/SDL2.dll" "${HECT_OUTPUT_DIR}/SDL2.dll"
                )
        endif()
    endif()
    find_package(SDL2 REQUIRED)
    target_include_directories(Hect PRIVATE ${SDL2_INCLUDE_DIR})
    target_link_libraries(Hect PRIVATE ${SDL2_LIBRARY})
endif()

# Add platform-specific dependencies
if(MSVC)
    target_link_libraries(Hect PRIVATE ws2_32 winmm)
else()
    target_link_libraries(Hect PRIVATE pthread)
endif()

set_target_properties(Hect PROPERTIES
    PROJECT_LABEL Hect
    LINKER_LANGUAGE CXX
    FOLDER "/Engine"
    )
