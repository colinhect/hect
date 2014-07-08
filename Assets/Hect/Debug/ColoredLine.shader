{
    "modules" :
    [
        "Hect/Debug/ColoredLine.frag",
        "Hect/Debug/ColoredLine.vert"
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