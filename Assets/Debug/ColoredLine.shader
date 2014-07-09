{
    "modules" :
    [
        "Debug/ColoredLine.frag",
        "Debug/ColoredLine.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "modelViewProjection",
            "binding" : "ModelViewProjectionMatrix"
        },
        {
            "name" : "colorOverride",
            "defaultValue" :
            {
                "type" : "Vector3",
                "value" : [ 0.0, 0.0, 0.0 ]
            }
        }
    ]
}