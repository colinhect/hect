{
    "modules" :
    [
        "GeometryBuffer.frag",
        "Environment.frag",
        "Screen.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "cameraPosition",
            "binding" : "CameraPosition"
        },
        {
            "name" : "diffuseBuffer",
            "type" : "Texture",
            "value" : 0
        },
        {
            "name" : "materialBuffer",
            "type" : "Texture",
            "value" : 1
        },
        {
            "name" : "positionBuffer",
            "type" : "Texture",
            "value" : 2
        },
        {
            "name" : "normalBuffer",
            "type" : "Texture",
            "value" : 3
        },
        {
            "name" : "lightProbeTexture",
            "type" : "Texture",
            "value" : 4
        }
    ]
}