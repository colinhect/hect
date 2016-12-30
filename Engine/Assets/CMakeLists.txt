project(HectAssets)

include(Sources.cmake)

add_custom_target(HectAssets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR} ${OUTPUT_DIR}/Hect.data/
    SOURCES ${SOURCE_FILES}
    )

set_target_properties(HectAssets PROPERTIES
    PROJECT_LABEL HectAssets
    FOLDER "/Engine"
    )
