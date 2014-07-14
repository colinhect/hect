{
    "modules" :
    [
        "SkyBox.frag",
        "SkyBox.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "modelView",
            "binding" : "ModelViewMatrix"
        },
        {
            "name" : "modelViewProjection",
            "binding" : "ModelViewProjectionMatrix"
        },
        {
            "name" : "skyBoxCubeMap",
            "type" : "Texture",
            "value" : 0
        }
    ]
}