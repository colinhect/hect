{
    "modules": [
        {
            "type": "Vertex",
            "path": "Screen.glsl"
        },
        {
            "type": "Fragment",
            "path": "DirectionalLight.glsl"
        }
    ],
    "parameters": [
        {
            "type": "Vector3",
            "name": "lightColor",
            "value": [ 1, 1, 1 ]
        },
        {
            "type": "Vector3",
            "name": "lightDirection",
            "value": [ 1, 0, 0 ]
        },
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
        }
    ]
}