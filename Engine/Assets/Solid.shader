{
    "modules": [
        {
            "type": "Vertex",
            "path": "Solid.Vertex.glsl"
        },
        {
            "type": "Fragment",
            "path": "Solid.Fragment.glsl"
        }
    ],
    "parameters": [
        {
            "type": "Vector3",
            "name": "diffuse",
            "value": [ 1, 1, 1 ]
        },
        {
            "type": "Float",
            "name": "roughness",
            "value": 1.0
        },
        {
            "type": "Float",
            "name": "metallic",
            "value": 1.0
        },
        {
            "name": "model",
            "binding": "ModelMatrix"
        },
        {
            "name": "modelViewProjection",
            "binding": "ModelViewProjectionMatrix"
        }
    ]
}