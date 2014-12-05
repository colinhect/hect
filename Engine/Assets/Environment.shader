{
    "modules": [
        {
            "type": "Vertex",
            "path": "Screen.glsl"
        },
        {
            "type": "Fragment",
            "path": "Environment.glsl"
        }
    ],
    "parameters": [
        {
            "name": "cameraPosition",
            "binding": "CameraPosition"
        },
        {
            "type": "Texture",
            "name": "diffuseBuffer"
        },
        {
            "type": "Texture",
            "name": "materialBuffer"
        },
        {
            "type": "Texture",
            "name": "positionBuffer"
        },
        {
            "type": "Texture",
            "name": "normalBuffer"
        },
        {
            "type": "Texture",
            "name": "lightProbeTexture"
        }
    ]
}