{
    "modules": [
        "GeometryBuffer.frag",
        "Environment.frag",
        "Screen.vert"
    ],
    "uniforms": [
        {
            "name": "cameraPosition",
            "binding": "CameraPosition"
        },
        {
            "name": "diffuseBuffer",
            "value": 0,
            "type": "Texture"
        },
        {
            "name": "materialBuffer",
            "value": 1,
            "type": "Texture"
        },
        {
            "name": "positionBuffer",
            "value": 2,
            "type": "Texture"
        },
        {
            "name": "normalBuffer",
            "value": 3,
            "type": "Texture"
        },
        {
            "name": "lightProbeTexture",
            "value": 4,
            "type": "Texture"
        }
    ]
}