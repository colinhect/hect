{
    "modules": [
        {
            "type": "Vertex",
            "path": "Screen.glsl"
        },
        {
            "type": "Fragment",
            "path": "Exposure.glsl"
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
            "name": "accumulationBuffer",
            "value": 0
        }
    ]
}