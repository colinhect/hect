{
    "modules" :
    [
        "Triplanar.frag",
        "Triplanar.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "diffuseTexture",
            "type" : "Texture",
            "value" : 0
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