{
    "modules": [
        {
            "type": "Vertex",
            "source": "Solid.glsl"
        },
        {
            "type": "Fragment",
            "source": "Solid.glsl"
        }
    ],
    "uniforms": [
        {
            "name": "diffuse",
            "value": [
                1,
                1,
                1
            ],
            "type": "Vector3"
        },
        {
            "name": "roughness",
            "value": 1.0,
            "type": "Float"
        },
        {
            "name": "metallic",
            "value": 1.0,
            "type": "Float"
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