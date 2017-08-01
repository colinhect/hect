project(Build)

include(Sources.cmake)

add_custom_target(Build
    SOURCES ${SOURCE_FILES}
    )

set_target_properties(Build PROPERTIES
    PROJECT_LABEL Build
    FOLDER "/Engine/Tools"
    )
