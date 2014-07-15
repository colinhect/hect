{
    "modules" :
    [
        "Triplanar.frag",
        "Triplanar.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "diffuseMap",
            "type" : "Texture",
            "value" : 0
        },
        {
            "name" : "materialMap",
            "type" : "Texture",
            "value" : 1
        },
        {
            "name" : "normalMap",
            "type" : "Texture",
            "value" : 2
        },
        {
            "name" : "textureScale",
            "type" : "Vector2",
            "value" : [ 1, 1 ]
        },
        {
            "name" : "model",
            "binding" : "ModelMatrix"
        },
        {
            "name" : "modelView",
            "binding" : "ModelViewMatrix"
        },
        {
            "name" : "modelViewProjection",
            "binding" : "ModelViewProjectionMatrix"
        }
    ]
}