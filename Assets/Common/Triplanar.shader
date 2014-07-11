{
    "modules" :
    [
        "Common/Triplanar.frag",
        "Common/Triplanar.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "albedoTexture",
            "type" : "Texture",
            "value" : 0
        },
        {
            "name" : "textureScale",
            "type" : "Vector2",
            "value" : [ 1, 1 ]
        },
        {
            "name" : "modelViewProjection",
            "binding" : "ModelViewProjectionMatrix"
        }
    ]
}