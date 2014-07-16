{
    "modules" :
    [
        "Solid.frag",
        "Solid.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "diffuse",
            "type" : "Vector3",
            "value" : [ 1, 1, 1 ]
        },
        {
            "name" : "roughness",
            "type" : "Float",
            "value" : 1.0
        },
        {
            "name" : "metallic",
            "type" : "Float",
            "value" : 1.0
        },
        {
            "name" : "model",
            "binding" : "ModelMatrix"
        },
        {
            "name" : "modelViewProjection",
            "binding" : "ModelViewProjectionMatrix"
        }
    ]
}