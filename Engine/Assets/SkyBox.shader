{
    "modules": [
        {
            "type": "Vertex",
            "path": "SkyBox.Vertex.glsl"
        },
        {
            "type": "Fragment",
            "path": "SkyBox.Fragment.glsl"
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