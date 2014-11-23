{
    "modules": [
        {
            "type": "Vertex",
            "path": "SkyBox.glsl"
        },
        {
            "type": "Fragment",
            "path": "Exposure.glsl"
        },
        {
            "type": "Fragment",
            "path": "SkyBox.glsl"
        }
    ],
    "parameters": [
        {
            "name": "exposure",
            "binding": "CameraExposure"
        },
        {
            "name": "oneOverGamma",
            "binding": "CameraOneOverGamma"
        },
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