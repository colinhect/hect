{
    "modules": [
        {
            "type": "Vertex",
            "source": "Triplanar.glsl"
        },
        {
            "type": "Fragment",
            "source": "Triplanar.glsl"
        }
    ],
    "uniforms": [
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