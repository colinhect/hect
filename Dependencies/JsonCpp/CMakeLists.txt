include_directories(
    "${PROJECT_SOURCE_DIR}/Dependencies/JsonCpp"
    )

add_library(JsonCpp STATIC
    "json_batchallocator.h"
    "json_internalarray.inl"
    "json_internalmap.inl"
    "json_reader.cpp"
    "json_tool.h"
    "json_value.cpp"
    "json_valueiterator.inl"
    "json_writer.cpp"
    "json/autolink.h"
    "json/config.h"
    "json/features.h"
    "json/forwards.h"
    "json/json.h"
    "json/reader.h"
    "json/value.h"
    "json/writer.h"
    )
    
set_target_properties(JsonCpp PROPERTIES
    PROJECT_LABEL JsonCpp
    FOLDER "/Dependencies"
    LINKER_LANGUAGE CXX
    )