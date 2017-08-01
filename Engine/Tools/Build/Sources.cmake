# Generated by Build Tool (see Engine/Tools/Build for details)

set(SOURCE
    "./generate_sources.py"
    "./generate_types.py"
    "./templates.py"
    "./__main__.py"
    )

source_group("Source" FILES ${SOURCE})

set(SOURCE_TEMPLATES
    "Templates/AddEnumValue.template"
    "Templates/CMakeSetGroup.template"
    "Templates/CMakeSetSourceFiles.template"
    "Templates/CMakeSetVar.template"
    "Templates/CMakeSourceFile.template"
    "Templates/CMakeSourceVar.template"
    "Templates/DecodePropertyEnum.template"
    "Templates/DecodePropertyValue.template"
    "Templates/DecodePropertyVector.template"
    "Templates/EncodePropertyEnum.template"
    "Templates/EncodePropertyValue.template"
    "Templates/EncodePropertyVector.template"
    "Templates/Include.template"
    "Templates/Layout.template"
    "Templates/RegisterClass.template"
    "Templates/RegisterComponent.template"
    "Templates/RegisterEnum.template"
    "Templates/RegisterScene.template"
    "Templates/RegisterSystem.template"
    "Templates/SetDecodeFunction.template"
    "Templates/SetEncodeFunction.template"
    )

source_group("Source\\Templates" FILES ${SOURCE_TEMPLATES})

set(SOURCE_FILES
    ${SOURCE}
    ${SOURCE_TEMPLATES}
    )

