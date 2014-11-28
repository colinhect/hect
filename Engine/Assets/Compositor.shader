{
    "modules": [
        {
            "type": "Vertex",
            "path": "Screen.glsl"
        },
        {
            "type": "Fragment",
            "path": "Compositor.glsl"
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
            "type": "Texture",
            "name": "diffuseBuffer",
            "value": 0
        },
        {
            "type": "Texture",
            "name": "accumulationBuffer",
            "value": 1
        }
    ]
}