{
    "modules": [
        {
            "type": "Vertex",
            "path": "Screen.glsl"
        },
        {
            "type": "Fragment",
            "path": "GeometryBuffer.glsl"
        },
        {
            "type": "Fragment",
            "path": "DirectionalLight.glsl"
        }
    ],
    "parameters": [
        {
            "name": "lightColor",
            "value": [
                1,
                1,
                1
            ],
            "type": "Vector3"
        },
        {
            "name": "lightDirection",
            "value": [
                1,
                0,
                0
            ],
            "type": "Vector3"
        },
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
        }
    ]
}