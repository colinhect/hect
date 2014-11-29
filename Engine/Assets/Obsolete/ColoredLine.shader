{
    "modules": [
        {
            "type": "Vertex",
            "path": "ColoredLine.glsl"
        },
        {
            "type": "Fragment", 
            "path": "ColoredLine.glsl"
        }
    ],
    "parameters": [
        {
            "name": "modelViewProjection",
            "binding": "ModelViewProjectionMatrix"
        },
        {
            "type": "Vector3",
            "name": "colorOverride",
            "value": [ 0, 0, 0 ]
        }
    ]
}