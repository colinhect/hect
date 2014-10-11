{
    "modules": [
        {
            "type": "Vertex",
            "source": "SkyBox.glsl"
        },
        {
            "type": "Fragment",
            "source": "SkyBox.glsl"
        }
    ],
    "uniforms": [
        {
            "name": "modelViewProjection",
            "binding": "ModelViewProjectionMatrix"
        },
        {
            "name": "skyBoxTexture",
            "value": 0,
            "type": "Texture"
        }
    ]
}