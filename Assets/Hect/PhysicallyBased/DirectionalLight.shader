{
    "modules" :
    [
        "DirectionalLight.frag",
        "Screen.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "color",
            "type" : "Vector3",
            "value" : [ 1, 1, 1 ]
        },
        {
            "name" : "direction",
            "type" : "Vector3",
            "value" : [ 1, 0, 0 ]
        },
        {
            "name" : "view",
            "type" : "Matrix4"
        },
        {
            "name" : "diffuseBuffer",
            "type" : "Texture",
            "value" : 0
        },
        {
            "name" : "specularBuffer",
            "type" : "Texture",
            "value" : 1
        },
        {
            "name" : "normalBuffer",
            "type" : "Texture",
            "value" : 2
        }
    ]
}