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
            "path": "RenderStages/LightAccumulation.glsl"
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
            "name": "diffuseBuffer",
            "value": 0
        },
        {
            "type": "Texture",
            "name": "materialBuffer",
            "value": 1
        },
        {
            "type": "Texture",
            "name": "positionBuffer",
            "value": 2
        },
        {
            "type": "Texture",
            "name": "normalBuffer",
            "value": 3
        },
        {
            "type": "Texture",
            "name": "lightProbeTexture",
            "value": 4
        }
    ]
}