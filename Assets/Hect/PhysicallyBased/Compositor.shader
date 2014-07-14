{
    "modules" :
    [
        "Compositor.frag",
        "Screen.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "exposure",
            "type" : "Float",
            "value" : 4.5
        },
        {
            "name" : "oneOverGamma",
            "type" : "Float",
            "value" : 0.4545
        },
        {
            "name" : "diffuseBuffer",
            "type" : "Texture",
            "value" : 0
        },
        {
            "name" : "accumulationBuffer",
            "type" : "Texture",
            "value" : 1
        }
    ]
}