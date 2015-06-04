project(HectTypes)

add_custom_target(HectTypes
    COMMAND python ${PROJECT_SOURCE_DIR}/../../../Tools/reflect.py ${PROJECT_SOURCE_DIR}/../ ${PROJECT_SOURCE_DIR} hect
    )
    
set_target_properties(HectTypes PROPERTIES
    PROJECT_LABEL HectTypes
    FOLDER "/Engine"
    )
