{
    "modules": [
        {
            "type": "Vertex",
            "path": "ColoredLine.Vertex.glsl"
        },
        {
            "type": "Fragment", 
            "path": "ColoredLine.Fragment.glsl"
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