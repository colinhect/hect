{
    "modules": [
        {
            "type": "Vertex",
            "path": "Screen.glsl"
        },
        {
            "type": "Fragment",
            "path": "Expose.glsl"
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
            "name": "backBuffer"
        }
    ]
}