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
            "path": "RenderStages/PhysicalLightAccumulation.glsl"
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
        }
    ]
}