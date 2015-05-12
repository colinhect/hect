project(HectAssets)

add_custom_target(HectAssets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR} ${OUTPUT_DIR}/Hect.data/
    )
    
set_target_properties(HectAssets PROPERTIES
    PROJECT_LABEL HectAssets
    FOLDER "/Engine"
    )
