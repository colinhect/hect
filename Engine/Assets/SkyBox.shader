{
    "modules": [
        {
            "type": "Vertex",
            "path": "SkyBox.glsl"
        },
        {
            "type": "Fragment",
            "path": "RenderStages/OpaqueGeometry.glsl"
        },
        {
            "type": "Fragment",
            "path": "SkyBox.glsl"
        }
    ],
    "parameters": [
        {
            "name": "modelViewProjection",
            "binding": "ModelViewProjectionMatrix"
        },
        {
            "type": "Texture",
            "name": "skyBoxTexture",
            "value": 0
        }
    ]
}