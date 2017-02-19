# Generated by Build Tool (see Engine/Tools/Build for details)

set(SOURCE_INTERFACE
    "Interface/Vera.font"
    )

source_group("Assets\\Interface" FILES ${SOURCE_INTERFACE})

set(SOURCE_MATERIALS
    "Materials/DebugLines.material.yaml"
    "Materials/Default.material.yaml"
    "Materials/Opaque.material.yaml"
    "Materials/OpaqueSolid.material.yaml"
    )

source_group("Assets\\Materials" FILES ${SOURCE_MATERIALS})

set(SOURCE_RENDERING
    "Rendering/Composite.glsl"
    "Rendering/Composite.shader.yaml"
    "Rendering/DirectionalLight.glsl"
    "Rendering/DirectionalLight.shader.yaml"
    "Rendering/Environment.glsl"
    "Rendering/Environment.shader.yaml"
    "Rendering/Expose.glsl"
    "Rendering/Expose.shader.yaml"
    "Rendering/Screen.glsl"
    "Rendering/SkyBox.mesh.yaml"
    )

source_group("Assets\\Rendering" FILES ${SOURCE_RENDERING})

set(SOURCE_SCENES
    "Scenes/Default.scene.yaml"
    )

source_group("Assets\\Scenes" FILES ${SOURCE_SCENES})

set(SOURCE_SHADERS
    "Shaders/Additive.Fragment.glsl"
    "Shaders/Additive.shader.yaml"
    "Shaders/Additive.Vertex.glsl"
    "Shaders/DebugLines.Fragment.glsl"
    "Shaders/DebugLines.shader.yaml"
    "Shaders/DebugLines.Vertex.glsl"
    "Shaders/Opaque.Fragment.glsl"
    "Shaders/Opaque.shader.yaml"
    "Shaders/Opaque.Vertex.glsl"
    "Shaders/OpaqueSolid.Fragment.glsl"
    "Shaders/OpaqueSolid.shader.yaml"
    "Shaders/OpaqueSolid.Vertex.glsl"
    "Shaders/SkyBox.Fragment.glsl"
    "Shaders/SkyBox.shader.yaml"
    "Shaders/SkyBox.Vertex.glsl"
    )

source_group("Assets\\Shaders" FILES ${SOURCE_SHADERS})

set(SOURCE_SHADERS_RENDERSTAGES
    "Shaders/RenderStages/PhysicalGeometry.glsl"
    "Shaders/RenderStages/PhysicalGeometry.shader.yaml"
    "Shaders/RenderStages/PostPhysicalGeometry.glsl"
    "Shaders/RenderStages/PostPhysicalGeometry.shader.yaml"
    "Shaders/RenderStages/PrePhysicalGeometry.glsl"
    "Shaders/RenderStages/PrePhysicalGeometry.shader.yaml"
    )

source_group("Assets\\Shaders\\RenderStages" FILES ${SOURCE_SHADERS_RENDERSTAGES})

set(SOURCE_FILES
    ${SOURCE_INTERFACE}
    ${SOURCE_MATERIALS}
    ${SOURCE_RENDERING}
    ${SOURCE_SCENES}
    ${SOURCE_SHADERS}
    ${SOURCE_SHADERS_RENDERSTAGES}
    )

