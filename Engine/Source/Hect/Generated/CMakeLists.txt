project(HectTypes)

add_custom_target(HectTypes)
    
set_target_properties(HectTypes PROPERTIES
    PROJECT_LABEL HectTypes
    FOLDER "/Engine"
    )

# Generate reflection code
add_custom_command(TARGET HectTypes
    PRE_BUILD
    COMMAND python ${PROJECT_SOURCE_DIR}/../../../Tools/reflect.py ${PROJECT_SOURCE_DIR}/../ ${PROJECT_SOURCE_DIR} hect
    )
