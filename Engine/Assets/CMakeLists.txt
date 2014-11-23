project(HectAssets)

add_custom_target(HectAssets)
    
set_target_properties(HectAssets PROPERTIES
    PROJECT_LABEL HectAssets
    FOLDER "/Engine"
    )

# Copy assets
add_custom_command(TARGET HectAssets
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR} ${OUTPUT_DIR}/Hect.data/
    )
