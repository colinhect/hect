{
    "modules": [
        {
            "type": "Vertex",
            "source": "ColoredLine.glsl"
        },
        {
            "type": "Fragment", 
            "source": "ColoredLine.glsl"
        }
    ],
    "uniforms": [
        {
            "name": "modelViewProjection",
            "binding": "ModelViewProjectionMatrix"
        },
        {
            "name": "colorOverride",
            "value": [
                0.0,
                0.0,
                0.0
            ],
            "type": "Vector3"
        }
    ]
}