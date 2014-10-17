{
    "modules": [
        {
            "type": "Vertex",
            "path": "Triplanar.glsl"
        },
        {
            "type": "Fragment",
            "path": "Triplanar.glsl"
        }
    ],
    "parameters": [
        {
            "name": "diffuseTexture",
            "value": 0,
            "type": "Texture"
        },
        {
            "name": "materialTexture",
            "value": 1,
            "type": "Texture"
        },
        {
            "name": "normalTexture",
            "value": 2,
            "type": "Texture"
        },
        {
            "name": "textureScale",
            "value": [
                1,
                1
            ],
            "type": "Vector2"
        },
        {
            "name": "model",
            "binding": "ModelMatrix"
        },
        {
            "name": "modelViewProjection",
            "binding": "ModelViewProjectionMatrix"
        }
    ]
}